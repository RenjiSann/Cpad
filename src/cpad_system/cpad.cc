#include <stack>

#include "cpah.hh"
#include "display.hh"
#include "entities/config.hh"
#include "entities/folder.hh"
#include "executor.hh"

namespace Cpad
{

    Cpad::Cpad(Config &cfg)
        : cfg_(&cfg)
        , cur_folder_(cfg.get_hierarchy().get())
    {}

    void Cpad::repl(std::map<std::string, ::Folder> &map,
                    const std::string &home_path)
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

            //auto task = Task(tokens);

            executor = Executor::instance().execute(map, current_folder, tokens,
                                                    home_path, combo,
                                                    cfg_->get_emoji_status());

            ::Display::instance().display_executor(executor, combo,
                                                   cfg_->get_emoji_status());

        } while (executor.first != Executor::ExecutionType::QUIT);
    }

} // namespace Cpad