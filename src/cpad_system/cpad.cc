#include <stack>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "cpah.hh"
#include "display.hh"
#include "entities/combo_command.hh"
#include "entities/command.hh"
#include "entities/config.hh"
#include "entities/element.hh"
#include "entities/folder.hh"
#include "executor.hh"

namespace Cpad
{

    Cpad::Cpad(Config &cfg)
        : cfg_(&cfg)
        , cur_folder_(cfg.get_hierarchy().get())
    {}

    void Cpad::repl()
    {
        // Enable/Disable emojis in display.
        Output::instance().set_emoji_status(cfg_->get_emoji_status());

        std::stack<std::string> last_folders;
        std::string current_folder = ".";
        Executor::executor_result executor;
        std::string command_input;

        do
        {
            ::Element combo;
            executor =
                Executor::executor_result(Executor::ExecutionType::CLEAR, "");

            // Display the current folder's content.
            Output::instance().display_folder(cur_folder_);

            // Ask for input.
            std::cout << "Choose your command "
                      << (cfg_->get_emoji_status() ? "➜" : ">") << " ";
            std::getline(std::cin, command_input);

            if (command_input.empty())
            {
                Output::instance().clear();
                continue;
            }

            // Scan the input and get the tokens.
            auto tokens = Scaner::instance().scan(command_input);

            // Check for error.
            auto error =
                ErrorHandling::instance().check_error(cur_folder_, tokens);

            // Interrupt REPL iteration if error.
            if (error != ErrorHandling::Error::NONE)
            {
                Output::instance().display_error(error);
                continue;
            }

            // Build the task from the tokens.
            auto task = Task(tokens, cur_folder_);

            // Handle QUIT event first.
            if (task.get_type() == Task::QUIT)
                break;

            // Execute the task.
            if (task.is_internal_task())
                this->process_task(task);
            else if (task.get_type() == Task::DISPLAY_HELP)
                Output::instance().display_help();
            else
            {
                // Command execution call.
                if (task.get_type() == Task::RUN_COMMAND)
                    Output::instance().run_command(
                        std::get<Command *>(task.get_data()));
                else
                    Output::instance().run_combo(
                        std::get<ComboCommand *>(task.get_data()));
            }

            // Display the result.
            Output::instance().display_process(task);

        } while (true);

        Output::instance().bye();
    }

    void Cpad::process_task(const Task &t)
    {
        switch (t.get_type())
        {
        case Task::BACK_FOLDER:
            if (cur_folder_->get_parent() == nullptr)
                throw std::runtime_error(
                    "Illegal state, cannot go up the root.");
            change_current_folder(cur_folder_->get_parent());
            break;
        case Task::GOTO_FOLDER:
            change_current_folder(std::get<Folder *>(t.get_data()));
            break;
        case Task::DELETE_CHILD:
            delete_child(std::get<int>(t.get_data()));
            break;
        case Task::CREATE_COMMAND:
            create_command(std::get<std::string>(t.get_data()));
            break;
        case Task::CREATE_COMBO:
            create_combo(std::get<std::vector<std::string>>(t.get_data()));
            break;
        case Task::CREATE_FOLDER:
            create_folder(std::get<std::string>(t.get_data()));
            break;
        case Task::SWAP_ENTRIES:
            int i, j;
            std::tie(i, j) = std::get<std::pair<int, int>>(t.get_data());
            swap_entries(i, j);
            break;
        case Task::RESET_FOLDER:
            throw std::invalid_argument("Unimplemented feature: Folder reset.");
            break;
        case Task::RESET_ALL:
            throw std::invalid_argument(
                "Unimplemented feature: Full data deletion.");
            break;
        default:
            // This time should never happen.
            throw std::runtime_error("Illegal state.");
        }

        // After applying the task, save the config.
        cfg_->sync_file();
    }

    void Cpad::change_current_folder(Folder *f)
    {
        cur_folder_ = f;
    }

    void Cpad::swap_entries(int i, int j) const
    {
        cur_folder_->swap_children(i, j);
    }

    void Cpad::delete_child(int i) const
    {
        cur_folder_->remove_child(i);
    }

    void Cpad::create_folder(const std::string &name) const
    {
        cur_folder_->add_child(Folder::empty(name));
    }

    void Cpad::create_command(const std::string &cmd) const
    {
        cur_folder_->add_child(Command::from_string(cmd));
    }

    void Cpad::create_combo(const std::vector<std::string> &cmds) const
    {
        cur_folder_->add_child(ComboCommand::from_vec(cmds));
    }

} // namespace Cpad