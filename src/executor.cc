#include "executor.hh"

#include <iostream>
#include <memory>
#include <stack>
#include <stdlib.h>
#include <string>
#include <strings.h>
#include <tuple>
#include <unistd.h>
#include <vector>

#include "argument-type.hh"
#include "convertor.hh"
#include "entities/combo_command.hh"
#include "entities/command.hh"
#include "entities/element.hh"
#include "entities/folder.hh"
#include "json.hh"
#include "scaner.hh"

namespace Cpad
{
    /**
     * @brief Helper function to get the index from a token object.
     */
    int tok_to_index(const Tokens &toks)
    {
        return std::stoi(toks.second[1]);
    }

    /**
     * @brief Helper function to get the index pair from a token object.
     */
    std::pair<int, int> tok_to_index_pair(const Tokens &toks)
    {
        return { std::stoi(toks.second[1]), std::stoi(toks.second[2]) };
    }

    /**
     * @brief Helper function to get the string parameter from a token object.
     */
    std::string tok_to_sring(const Tokens &toks)
    {
        return toks.second[1];
    }

    /**
     * @brief Helper function to get the string vector parameter from a token
     * object.
     */
    std::vector<std::string> tok_to_sring_vec(const Tokens &toks)
    {
        std::vector<std::string> res(toks.second.size() - 1);
        res.insert(res.begin(), toks.second.begin() + 1, toks.second.end());
        return res;
    }

    /**
     * @brief Special case fucntion called when
     *
     * @param toks
     * @return std::pair<Task::TaskType, Task::TaskData>
     */
    std::pair<Task::TaskType, Task::TaskData> tok_to_exec(const Tokens &toks,
                                                          const Folder *folder)
    {
        if (toks.second[0] == "b")
            return { Task::BACK_FOLDER, -1 };
        else if (toks.second[0] == "h")
            return { Task::DISPLAY_HELP, -1 };
        else if (toks.second[0] == "q")
            return { Task::QUIT, -1 };

        // In the last case, get the command or combo or folder in the
        // corresponding index.
        auto index = std::stoi(toks.second[0]);
        auto child = folder->get_children()[index];

        switch (child->get_type())
        {
        case Element::COMMAND:
            return { Task::RUN_COMMAND, index };
        case Element::COMBO_COMMAND:
            return { Task::RUN_COMBO, index };
        case Element::FOLDER:
            return { Task::GOTO_FOLDER, index };
        }
    }

    Task::Task(const Tokens &toks, const Folder *folder)
    {
        switch (toks.first)
        {
        case ArgumentType::ELEMENT_TYPE::EXECUTION:
            std::tie(type_, data_) = tok_to_exec(toks, folder);
            break;
        case ArgumentType::ELEMENT_TYPE::DELETE:
            type_ = DELETE_CHILD;
            data_ = tok_to_index(toks);
            break;
        case ArgumentType::ELEMENT_TYPE::RESET_FOLDER:
            type_ = RESET_FOLDER;
            data_ = tok_to_index(toks);
            break;
        case ArgumentType::ELEMENT_TYPE::CREATE_COMMAND:
            type_ = CREATE_COMMAND;
            data_ = tok_to_sring(toks);
            break;
        case ArgumentType::ELEMENT_TYPE::CREATE_FOLDER:
            type_ = CREATE_FOLDER;
            data_ = tok_to_sring(toks);
            break;
        case ArgumentType::ELEMENT_TYPE::CREATE_COMBO:
            type_ = CREATE_COMBO;
            data_ = tok_to_sring_vec(toks);
            break;
        case ArgumentType::ELEMENT_TYPE::MOVE:
            type_ = SWAP_ENTRIES;
            data_ = tok_to_index_pair(toks);
            break;
        case ArgumentType::ELEMENT_TYPE::RESET_ALL:
            type_ = RESET_ALL;
            break;
        }
    }

    bool Task::is_help_or_exec() const
    {
        return type_ == DISPLAY_HELP || type_ == RUN_COMBO
            || type_ == RUN_COMMAND;
    }

} // namespace Cpad

/*






















*/

Element &Executor::get_element_from_index(std::map<std::string, Folder> &map,
                                          const std::string &current_folder,
                                          size_t index)
{
    return map[current_folder].get_elements()[index];
}

Executor::executor_result
Executor::execute_execution(std::map<std::string, Folder> &map,
                            const std::string &current_folder, Tokens &tokens,
                            Element &element_combo)
{
    if (tokens.second[0] == "h")
        return executor_result(ExecutionType::DISPLAY_HELP, "");
    else if (tokens.second[0] == "q")
        return executor_result(ExecutionType::QUIT, "");
    else if (tokens.second[0] == "b")
        return executor_result(ExecutionType::BACK_FOLDER, "");

    size_t input_value = std::stoi(tokens.second[0]);
    auto element = get_element_from_index(map, current_folder, input_value - 1);

    if (element.get_is_folder())
        return executor_result(ExecutionType::MOVE_FOLDER, element.get_name());
    else if (element.get_is_combo())
    {
        element_combo = element;
        return executor_result(ExecutionType::COMBO_EXECUTION,
                               element.get_name());
    }
    return executor_result(ExecutionType::COMMAND, element.get_name());
}

Executor::executor_result
Executor::execute_create_command(std::map<std::string, Folder> &map,
                                 const std::string &current_folder,
                                 Tokens &tokens)
{
    Convertor::instance().add_command(map, current_folder, tokens.second[1]);
    return executor_result(Executor::ExecutionType::CREATE_COMMAND,
                           tokens.second[1]);
}

Executor::executor_result
Executor::execute_create_folder(std::map<std::string, Folder> &map,
                                const std::string &current_folder,
                                Tokens &tokens)
{
    Convertor::instance().add_folder(map, current_folder, tokens.second[1]);
    return executor_result(Executor::ExecutionType::CREATE_FOLDER,
                           tokens.second[1]);
}

Executor::executor_result
Executor::execute_delete(std::map<std::string, Folder> &map,
                         const std::string &current_folder, Tokens &tokens)
{
    size_t input_value = std::stoi(tokens.second[1]);
    auto element = get_element_from_index(map, current_folder, input_value - 1);
    ExecutionType execution;
    if (element.get_is_folder())
    {
        Convertor::instance().remove_folder(map, current_folder,
                                            element.get_name());
        execution = Executor::ExecutionType::DELETE_FOLDER;
    }
    else if (element.get_is_combo())
    {
        Convertor::instance().remove_command(map, current_folder, input_value);
        execution = Executor::ExecutionType::DELETE_COMBO;
    }
    else
    {
        Convertor::instance().remove_command(map, current_folder, input_value);
        execution = Executor::ExecutionType::DELETE_COMMAND;
    }
    return executor_result(execution, element.get_name());
}

Executor::executor_result
Executor::execute_move(std::map<std::string, Folder> &map,
                       const std::string &current_folder, Tokens &tokens)
{
    size_t src_index_val = std::stoi(tokens.second[1]) - 1;
    size_t dst_index_val = std::stoi(tokens.second[2]) - 1;
    Convertor::instance().move(map, current_folder, src_index_val,
                               dst_index_val);
    return executor_result(Executor::ExecutionType::MOVE,
                           tokens.second[1] + " " + tokens.second[2]);
}

Executor::executor_result
Executor::execute_reset_folder(std::map<std::string, Folder> &map,
                               const std::string &current_folder,
                               Tokens &tokens)
{
    size_t value = std::stoi(tokens.second[1]) - 1;
    auto element = get_element_from_index(map, current_folder, value);
    Convertor::instance().reset_folder(map, element.get_name());
    return executor_result(Executor::ExecutionType::RESET_FOLDER,
                           element.get_name());
}

Executor::executor_result
Executor::execute_reset_all(std::map<std::string, Folder> &map)
{
    Convertor::instance().reset_all(map);
    return executor_result(Executor::ExecutionType::RESET_ALL, "");
}

Executor::executor_result
Executor::execute_create_combo(std::map<std::string, Folder> &map,
                               const std::string &current_folder,
                               Tokens &tokens)
{
    std::string combo_name;
    auto token_name = tokens.second[1];
    size_t token_size = token_name.size();

    if (token_name.size() >= 3 && token_name[0] == '{'
        && token_name[token_size - 1] == '}')
        combo_name = token_name.substr(1, token_size - 2);

    Element combo;
    if (!combo_name.empty())
        combo.set_name(combo_name);

    size_t i = combo_name.empty() ? 1 : 2;
    for (; i < tokens.second.size(); i++)
        combo.get_combo_elements_().push_back(Element(tokens.second[i], false));

    Convertor::instance().combo(map, current_folder, combo);
    return executor_result(Executor::ExecutionType::CREATE_COMBO, combo_name);
}

Executor::executor_result Executor::execute(std::map<std::string, Folder> &map,
                                            const std::string &current_folder,
                                            Tokens &tokens,
                                            const std::string &home_path,
                                            Element &element, bool emoji)
{
    ArgumentType::ELEMENT_TYPE element_type =
        ArgumentType::instance().convert_to_element_type(tokens.second[0]);

    executor_result execution_type;
    switch (element_type)
    {
    case ArgumentType::ELEMENT_TYPE::EXECUTION:
        execution_type =
            execute_execution(map, current_folder, tokens, element);
        break;
    case ArgumentType::ELEMENT_TYPE::CREATE_COMMAND:
        execution_type = execute_create_command(map, current_folder, tokens);
        break;
    case ArgumentType::ELEMENT_TYPE::CREATE_FOLDER:
        execution_type = execute_create_folder(map, current_folder, tokens);
        break;
    case ArgumentType::ELEMENT_TYPE::DELETE:
        execution_type = execute_delete(map, current_folder, tokens);
        break;
    case ArgumentType::ELEMENT_TYPE::MOVE:
        execution_type = execute_move(map, current_folder, tokens);
        break;
    case ArgumentType::ELEMENT_TYPE::RESET_FOLDER:
        execution_type = execute_reset_folder(map, current_folder, tokens);
        break;
    case ArgumentType::ELEMENT_TYPE::RESET_ALL:
        execution_type = execute_reset_all(map);
        break;
    case ArgumentType::ELEMENT_TYPE::CREATE_COMBO:
        execution_type = execute_create_combo(map, current_folder, tokens);
        break;
    }
    Convertor::instance().write(map, home_path, emoji);
    return execution_type;
}