#include "display.hh"

#include <iostream>
#include <unistd.h>

#include "entities/combo_command.hh"
#include "entities/element.hh"
#include "error-handling.hh"

namespace Cpad
{

    void Output::display_folder(Folder *folder) const
    {
        std::cout << (emoji_ ? "📁 : " : "Current folder: ") << BOLDGREEN
                  << folder->get_name() << std::endl
                  << std::endl;
        std::cout << RESET;

        // Display every children of the current folder.
        int displayed_index = 1;
        for (const auto &elm : folder->get_children())
        {
            std::cout << BOLD << std::to_string(displayed_index) << RESET
                      << (emoji_ ? " ➜ " : " - ");

            switch (elm->get_type())
            {
            case Element::FOLDER:
                std::cout << BOLDBLUE << elm->to_str() << (emoji_ ? " 📁 " : "")
                          << std::endl;
                break;
            case Element::COMMAND:
                std::cout << elm->to_str() << std::endl;
                break;
            case Element::COMBO_COMMAND:
                const ComboCommand &combo = dynamic_cast<ComboCommand &>(*elm);
                // Display the first item of the combo along with the index of
                // the element.
                std::cout << combo.get_commands()[0] << std::endl;

                // Then display all the following.
                for (auto cm = combo.get_commands().begin() + 1;
                     cm != combo.get_commands().end(); ++cm)
                {
                    std::cout << ' ' << RESET << (emoji_ ? " 🔸 " : " - ") << *cm
                              << std::endl;
                }
            }
            std::cout << RESET;
            displayed_index++;
        }

        std::cout << BOLDWHITE << "\n---" << std::endl;

        // Display the (b) button option to let the user know he can
        // climb up the hierarchy.
        if (folder->get_parent() != nullptr)
            std::cout << BOLDGREEN << "b- .." << std::endl;

        std::cout << YELLOW << "h- help" << std::endl;

        std::cout << RED << "q- quit " << (emoji_ ? "😥" : "") << std::endl;
        std::cout << RESET << std::endl;
    }

    void Output::display_error(ErrorHandling::Error error) const
    {
        // Get the corresponding error message.
        const std::string &err = error_to_str(error);

        std::cout << BOLDRED << (emoji_ ? "❌" : "") << err << std::endl;
        std::cout << YELLOW << "     Press " << BOLDGREEN << "h" << YELLOW
                  << " to see the documentation." << RESET << '\n'
                  << std::endl;
    }

    void Output::display_help() const
    {
        std::cout << BOLDCYAN << "-------- HELPER PAGE --------\n\n" << RESET;

        // -ac
        std::cout << BOLDGREEN << "-ac" << RESET
                  << ": Add a new command to the current file\n";
        std::cout << BOLDWHITE << "\t usage: -ac new_command\n" << RESET;
        std::cout << YELLOW << "\t exemple: -ac ls -a\n\n" << RESET;

        // -cb
        std::cout
            << BOLDGREEN << "-cb" << RESET
            << ": Allows you to create a new command combo, allowing you to "
               "launch several commands in a row by pressing a single key\n";
        std::cout << BOLDWHITE
                  << "\t usage: -cb command_1 [cb] command_2 [cb] command_n \n"
                  << RESET;
        std::cout << BOLDWHITE
                  << "\t usage: -cb {combo_name} command_1 [cb] command_2 "
                     "[cb] command_n \n"
                  << RESET;
        std::cout << YELLOW << "\t exemple: -cb ls -a [cb] pwd [cb] echo [?] \n"
                  << RESET;
        std::cout
            << YELLOW
            << "\t exemple: -cb {PUSH} git add [?] [cb] git status [cb] git "
               "commit -m \"[?]\" [cb] git push origin [?]\n\n"
            << RESET;

        // -af
        std::cout << BOLDGREEN << "-af" << RESET
                  << ": Add a new folder in de current directory\n";
        std::cout << BOLDWHITE << "\t usage: -af new_folder\n" << RESET;
        std::cout << YELLOW << "\t exemple: -af git commands\n\n" << RESET;

        // -de
        std::cout << BOLDGREEN << "-de" << RESET
                  << ": Delete a command or a folder\n";
        std::cout << BOLDWHITE << "\t usage: -de number_command_or_folder\n"
                  << RESET;
        std::cout << YELLOW << "\t exemple: -de 5\n\n" << RESET;

        // -rf
        std::cout << BOLDGREEN << "-rf" << RESET
                  << ": Reset the contents of a folder\n";
        std::cout << BOLDWHITE << "\t usage: -rf number_of_the_folder\n"
                  << RESET;
        std::cout << YELLOW << "\t exemple: -rf 5\n\n" << RESET;

        // -ra
        std::cout << BOLDGREEN << "-ra" << RESET
                  << ": Reset the contents of the entire folder\n";
        std::cout << BOLDWHITE << "\t usage: -ra \n" << RESET;

        // -mv
        std::cout << BOLDGREEN << "-mv" << RESET
                  << ": Move element to another position\n";
        std::cout << BOLDWHITE
                  << "\t usage: -mv source_position destination_position \n"
                  << RESET;
        std::cout << YELLOW << "\t exemple: -mv 2 5\n\n" << RESET;

        // [?]
        std::cout << BOLDGREEN << "[?]" << RESET
                  << ": If you add [?] during the creation of your command, "
                     "during the execution you will have the possibility to "
                     "replace them by the content of your choice\n";
        std::cout << BOLDWHITE << "\t usage: [?]\n" << RESET;
        std::cout << YELLOW << "\t exemple: -ac git commit -m \"[?]\"\n"
                  << RESET;
        std::cout << YELLOW << "\t exemple: -cb ls [?] [cb] cd [?] \n" << RESET;

        std::cout << BOLDWHITE << "\n---" << std::endl;
        std::cout << BOLDGREEN << "Press " << RESET << BOLDRED << "Enter"
                  << RESET << BOLDGREEN << " to back to Cpad" << std::endl;
        std::cout << RESET;
        std::cout << std::endl;
    }

    void Output::clear() const
    {
        std::cout << CLEAR_SCREEN;
    }

    void Output::set_emoji_status(bool v)
    {
        emoji_ = v;
    }

    std::ostream &operator<<(std::ostream &os, Color c)
    {
        static const char *colors[] = {

            "\033[0m",
            "\033[1m",
            "\033[37m",
            "\033[1m\033[34m" /* Bold Blue */,
            "\033[1m\033[31m" /* Bold Red */,
            "\033[4m\033[1m\033[31m" /* Underligned Bold Red */,
            "\033[31m" /* Red */,
            "\033[1m\033[32m" /* Bold Green */,
            "\033[0m\033[32m" /*Green */,
            "\033[33m" /* Yellow */,
            "\033[1m\033[37m" /* Bold White */,
            "\033[1m\033[36m" /* Bold Cyan */,
            "\033[1m\033[35m" /* Bold Magenta */
        };
        return os << colors[c];
    }

} // namespace Cpad

/*
































*/

void Display::execute(std::string &command, bool emoji, bool clear)
{
    if (is_template(command))
        remplace_templates(command, clear);

    std::cout << COL_BOLDGREEN + (emoji ? std::string("🔧 ") : "")
            + "Execution of: "
              << COL_RESET << command << std::endl;
    std::cout << "---\n";
    if (!cd_exec(command))
        system(command.c_str());
    std::cout << "---\n\n";
}

void Display::execute_combo(const std::string &command,
                            std::string &display_line, Element &element_combo,
                            bool emoji)
{
    set_display_line(display_line, "COMBO EXECUTION:", "✔️ ", "",
                     COL_BOLDGREEN, emoji);
    std::cout << COL_RESET << display_line << COL_RESET << COL_WHITE << ' '
              << command << COL_RESET << '\n'
              << std::endl;
    for (auto &command : element_combo.get_combo_elements_())
    {
        Executor::executor_result exec =
            Executor::executor_result(Exec::COMMAND, command.get_name());
        display_executor(exec, command, emoji, false);
    }
}

void Display::set_display_line(std::string &display_line,
                               const std::string &end_str,
                               const std::string &emoji_str,
                               const std::string &replace,
                               const std::string &color, bool emoji)
{
    if (emoji)
        display_line = color + emoji_str + end_str;
    else
        display_line = color + replace + end_str;
    display_line += COL_RESET;
}

bool Display::cd_exec(const std::string &command)
{
    std::stringstream ss(command);
    std::string token;
    ss >> token;
    if (token == "cd")
    {
        ss >> token;
        if (command.size() == 2)
            token = std::string(getenv("HOME"));
        chdir(token.c_str());
        return true;
    }
    return false;
}

bool Display::is_template(const std::string &command)
{
    if (command == "[?]")
        return true;
    return command.find("[?]") != std::string::npos;
}

void Display::remplace_templates(std::string &command, bool clear)
{
    size_t index = 1;

    while (true)
    {
        if (command.find("[?]", 0) == std::string::npos)
            break;
        std::string user_input;

        std::cout << COL_BOLDGREEN << "current_command: " << COL_RESET
                  << command << "\n";
        std::cout << index++ << " ➜ remplace " << COL_RED << "[?]" << COL_RESET
                  << " ➜ ";
        std::getline(std::cin, user_input);
        std::cout << '\n';
        command.replace(command.find("[?]"), 3, user_input);
        if (clear)
            system("clear");
    }
}

void Display::display_executor(Executor::executor_result executor,
                               Element &element_combo, bool emoji, bool clear)
{
    if (clear)
        system("clear");
    std::string display_line;
    switch (executor.first)
    {
    case Executor::ExecutionType::COMMAND:
        return execute(executor.second, emoji, clear);
    case Executor::ExecutionType::CREATE_COMMAND:
        set_display_line(display_line, "Creation of command:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::CREATE_COMBO:
        set_display_line(display_line, "Creation of combo:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::CREATE_FOLDER:
        set_display_line(display_line, "Creation of the folder:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::DELETE_COMMAND:
        set_display_line(display_line, "Deletion of command:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::DELETE_FOLDER:
        set_display_line(display_line, "Deletion of folder:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::DELETE_COMBO:
        set_display_line(display_line, "Deletion of combo:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::BACK_FOLDER:
        set_display_line(display_line, "You have returned to your old file",
                         "✔️ ", "", COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::MOVE_FOLDER:
        set_display_line(display_line, "You move into the folder:", "✔️ ",
                         "", COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::RESET_FOLDER:
        set_display_line(display_line, "You reset the folder:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::MOVE:
        set_display_line(display_line, "You move elements:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::RESET_ALL:
        set_display_line(display_line, "You reset all folders:", "✔️ ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::DISPLAY_HELP:
        display_helper();
        std::cout << "➜ ";
        std::getline(std::cin, display_line);
        system("clear");
        return;
    case Executor::ExecutionType::QUIT:
        set_display_line(display_line, "See you soon !", "👋 ", "",
                         COL_BOLDGREEN, emoji);
        break;
    case Executor::ExecutionType::COMBO_EXECUTION:
        return execute_combo(executor.second, display_line, element_combo,
                             emoji);
    case Executor::ExecutionType::CLEAR:
        set_display_line(display_line, "CLEAR !", "✔️ ", "", COL_BOLDGREEN,
                         emoji);
        break;
    }
    std::cout << COL_RESET << display_line << COL_RESET << COL_WHITE << ' '
              << executor.second << COL_RESET << '\n'
              << std::endl;
}

void Display::display_helper()
{
    std::cout << COL_BOLDCYAN << "-------- HELPER PAGE --------\n\n"
              << COL_RESET;
    std::cout << COL_BOLDGREEN << "-ac" << COL_RESET
              << ": Add a new command to the current file\n";
    std::cout << COL_BOLDWHITE << "\t usage: -ac new_command\n" << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -ac ls -a\n\n" << COL_RESET;

    std::cout << COL_BOLDGREEN << "-cb" << COL_RESET
              << ": Allows you to create a new command combo, allowing you to "
                 "launch several commands in a row by pressing a single key\n";
    std::cout << COL_BOLDWHITE
              << "\t usage: -cb command_1 [cb] command_2 [cb] command_n \n"
              << COL_RESET;
    std::cout << COL_BOLDWHITE
              << "\t usage: -cb {combo_name} command_1 [cb] command_2 "
                 "[cb] command_n \n"
              << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -cb ls -a [cb] pwd [cb] echo [?] \n"
              << COL_RESET;
    std::cout << COL_YELLOW
              << "\t exemple: -cb {PUSH} git add [?] [cb] git status [cb] git "
                 "commit -m \"[?]\" [cb] git push origin [?]\n\n"
              << COL_RESET;

    std::cout << COL_BOLDGREEN << "-af" << COL_RESET
              << ": Add a new folder in de current directory\n";
    std::cout << COL_BOLDWHITE << "\t usage: -af new_folder\n" << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -af git commands\n\n" << COL_RESET;

    std::cout << COL_BOLDGREEN << "-de" << COL_RESET
              << ": Delete a command or a folder\n";
    std::cout << COL_BOLDWHITE << "\t usage: -de number_command_or_folder\n"
              << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -de 5\n\n" << COL_RESET;

    std::cout << COL_BOLDGREEN << "-rf" << COL_RESET
              << ": Reset the contents of a folder\n";
    std::cout << COL_BOLDWHITE << "\t usage: -rf number_of_the_folder\n"
              << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -rf 5\n\n" << COL_RESET;

    std::cout << COL_BOLDGREEN << "-ra" << COL_RESET
              << ": Reset the contents of the entire folder\n";
    std::cout << COL_BOLDWHITE << "\t usage: -ra \n" << COL_RESET;

    std::cout << COL_BOLDGREEN << "-mv" << COL_RESET
              << ": Move element to another position\n";
    std::cout << COL_BOLDWHITE
              << "\t usage: -mv source_position destination_position \n"
              << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -mv 2 5\n\n" << COL_RESET;

    std::cout << COL_BOLDGREEN << "[?]" << COL_RESET
              << ": If you add [?] during the creation of your command, "
                 "during the execution you will have the possibility to "
                 "replace them by the content of your choice\n";
    std::cout << COL_BOLDWHITE << "\t usage: [?]\n" << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -ac git commit -m \"[?]\"\n"
              << COL_RESET;
    std::cout << COL_YELLOW << "\t exemple: -cb ls [?] [cb] cd [?] \n"
              << COL_RESET;

    std::cout << COL_BOLDWHITE << "\n---" << std::endl;
    std::cout << COL_BOLDGREEN << "Press " << COL_RESET << COL_BOLDRED
              << "Enter" << COL_RESET << COL_BOLDGREEN << " to back to Cpad"
              << std::endl;
    std::cout << COL_RESET;
    std::cout << std::endl;
}