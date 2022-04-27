
#include "arguments.hh"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "convertor.hh"
#include "executor.hh"

std::pair<std::string, bool>
get_element_from_index(std::map<std::string, Folder> &map, std::string &key,
                       size_t index)
{
    auto current = map[key].get_elements()[index];
    std::pair<std::string, bool> result{ current.get_name(),
                                         current.get_is_folder() };
    return result;
}
bool is_command(std::string &cmd)
{
    return cmd == "-ac" || cmd == "-af" || cmd == "-de" || cmd == "-rf"
        || cmd == "-ra" || cmd == "-mv" || cmd == "-cb";
}

void concat_argument(std::stringstream &ss, std::string token,
                     std::string &cmd_argument)
{
    size_t count = 0;
    while (ss >> token)
    {
        if (count != 0)
            cmd_argument += ' ';
        cmd_argument += token;
        count++;
    }
}

void parse_arg(std::string &arg, std::map<std::string, Folder> &map,
               std::string &key, std::string &home_path,
               Executor::ExecutionType &exec_type)
{
    std::stringstream ss(arg);
    std::string token;
    std::string cmd_argument;
    ss >> token;
    if (token == "-ac")
    {
        concat_argument(ss, token, cmd_argument);
        if (cmd_argument.empty())
        {
            exec_type = Executor::ExecutionType::EMPTY_NAME;
            return;
        }
        for (auto elm : map[key].get_elements())
        {
            if (elm.get_name() == cmd_argument)
            {
                exec_type = Executor::ExecutionType::COMMAND_ERROR_NAME;
                return;
            }
        }
        arg = cmd_argument;
        exec_type = Executor::ExecutionType::CREATE_COMMAND;
        Convertor::instance().add_command(map, key, cmd_argument);
    }
    else if (token == "-af")
    {
        concat_argument(ss, token, cmd_argument);
        if (cmd_argument.empty())
        {
            exec_type = Executor::ExecutionType::EMPTY_NAME;
            return;
        }
        for (auto elm : map[key].get_elements())
        {
            if (elm.get_name() == cmd_argument)
            {
                exec_type = Executor::ExecutionType::FOLDER_ERROR_NAME;
                return;
            }
        }
        arg = cmd_argument;
        exec_type = Executor::ExecutionType::CREATE_FOLDER;
        Convertor::instance().add_folder(map, key, cmd_argument);
    }
    else if (token == "-de")
    {
        std::string input_number;
        ss >> input_number;
        size_t value = std::stoi(input_number) - 1;

        auto element = get_element_from_index(map, key, value);
        arg = element.first;
        if (element.second)
        {
            exec_type = Executor::ExecutionType::DELETE_FOLDER;
            Convertor::instance().remove_folder(map, key, element.first);
        }
        else
        {
            exec_type = Executor::ExecutionType::DELETE_COMMAND;
            Convertor::instance().remove_command(map, key, value + 1);
        }
    }
    else if (token == "-rf")
    {
        std::string input_number;
        ss >> input_number;
        size_t value = std::stoi(input_number) - 1;

        auto element = get_element_from_index(map, key, value);
        arg = element.first;
        exec_type = Executor::ExecutionType::RESET_FOLDER;
        Convertor::instance().reset_folder(map, element.first);
    }
    else if (token == "-ra")
    {
        exec_type = Executor::ExecutionType::RESET_ALL;
        Convertor::instance().reset_all(map);
    }
    else if (token == "-mv")
    {
        std::string src_index;
        std::string dst_index;
        ss >> src_index;
        ss >> dst_index;

        size_t src_index_val = std::stoi(src_index) - 1;
        size_t dst_index_val = std::stoi(dst_index) - 1;
        exec_type = Executor::ExecutionType::SWAP;
        Convertor::instance().move(map, key, src_index_val, dst_index_val);
    }
    Convertor::instance().write(map, home_path);
}