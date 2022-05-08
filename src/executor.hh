#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "entities/folder.hh"
#include "scaner.hh"
#include "singleton.hh"

namespace Cpad
{
    /**
     * @brief Build a task to be run
     */
    class Task
    {
    public:
        enum TaskType
        {
            RUN_COMMAND,
            RUN_COMBO,
            GOTO_FOLDER,

            DELETE_CHILD,
            RESET_FOLDER,

            CREATE_COMMAND,
            CREATE_FOLDER,
            CREATE_COMBO,

            RESET_ALL,

            SWAP_ENTRIES,

            BACK_FOLDER,
            DISPLAY_HELP,
            QUIT
        };
        using TaskData = std::variant<
            std::pair<int, int> /* SWAP_FOLDERS */,
            std::string /* RUN_CMD, CREATE_FOLDER, CREATE_COMMAND */,
            std::vector<std::string> /* RUN_COMBO, CREATE_COMBO */,
            int /* GOTO_FOLDER, DELETE_CHILD,
                                        RESET_FOLDER */
            >;

    private:
        /// Define the type of task.
        TaskType type_;
        /// A space to hold the data for the task.
        TaskData data_;

    public:
        Task(const Tokens &tokens, const Folder *folder);

        bool is_help_or_exec() const;

        /* Getters/Setters */
        TaskType get_type() const;
        const TaskData &get_data() const;
    };

} // namespace Cpad

/*














*/

class Executor : public Singleton<Executor>
{
public:
    enum class ExecutionType
    {
        COMMAND,
        MOVE_FOLDER,
        BACK_FOLDER,
        COMBO_EXECUTION,

        CREATE_COMMAND,
        CREATE_FOLDER,
        CREATE_COMBO,

        DELETE_COMMAND,
        DELETE_FOLDER,
        DELETE_COMBO,

        RESET_FOLDER,
        RESET_ALL,

        MOVE,

        DISPLAY_HELP,
        CLEAR,
        QUIT
    };
    using executor_result = std::pair<ExecutionType, std::string>;

    executor_result execute(std::map<std::string, Folder> &map,
                            const std::string &current_folder, Tokens &tokens,
                            const std::string &home_path, Element &element,
                            bool emoji);

private:
    Element &get_element_from_index(std::map<std::string, Folder> &map,
                                    const std::string &current_folder,
                                    size_t index);

    executor_result execute_execution(std::map<std::string, Folder> &map,
                                      const std::string &current_folder,
                                      Tokens &tokens, Element &elem_combo);

    executor_result execute_create_command(std::map<std::string, Folder> &map,
                                           const std::string &current_folder,
                                           Tokens &tokens);

    executor_result execute_create_folder(std::map<std::string, Folder> &map,
                                          const std::string &current_folder,
                                          Tokens &tokens);
    executor_result execute_delete(std::map<std::string, Folder> &map,
                                   const std::string &current_folder,
                                   Tokens &tokens);
    executor_result execute_move(std::map<std::string, Folder> &map,
                                 const std::string &current_folder,
                                 Tokens &tokens);
    executor_result execute_reset_folder(std::map<std::string, Folder> &map,
                                         const std::string &current_folder,
                                         Tokens &tokens);
    executor_result execute_reset_all(std::map<std::string, Folder> &map);
    executor_result execute_create_combo(std::map<std::string, Folder> &map,
                                         const std::string &current_folder,
                                         Tokens &tokens);
};