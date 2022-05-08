#include "messages-manager.hh"

#include <algorithm>
#include <fstream>

#include "error-handling.hh"
#include "executor.hh"
#include "json-messages.hh"

nlohmann::json MessagesManager::read_json(const std::string &path)
{
    std::ifstream json_file(path);
    nlohmann::json json;

    // If the file is not open, it does not exist
    if (!json_file.is_open())
        throw std::ios_base::failure("Cannot access file: " + path);

    json_file >> json;
    json_file.close();
    return json;
}

void MessagesManager::from_json(const nlohmann::json &obj_j)
{
    std::cout << "oui\n";
    parse_messages(obj_j, true);
    parse_messages(obj_j, false);
}

void MessagesManager::parse_messages(const nlohmann::json &obj_j, bool is_error)
{
    auto error_dict = obj_j[is_error ? ERRORS_FIELD : EXECUTION_FIELD];
    auto emoji = nlohmann::json::string_t(error_dict[EMOJI_FIELD]);

    auto messages_dict = error_dict[MESSAGES_FIELD];
    for (const auto &element : messages_dict.get<nlohmann::json::object_t>())
    {
        auto str_english =
            nlohmann::json::string_t(element.second[ENGLISH_FIELD]);
        auto str_french =
            nlohmann::json::string_t(element.second[FRENCH_FIELD]);

        auto message_english = Message(emoji, str_english);
        auto message_french = Message(emoji, str_french);

        std::string str_key = nlohmann::json::string_t(element.first);
        is_error
            ? add_error_element(str_key, message_english, message_french)
            : add_execution_element(str_key, message_english, message_french);
    }
}

void MessagesManager::add_error_element(std::string &error_str,
                                        Message &message_english,
                                        Message &message_french)
{
    auto error = string_to_error(error_str);

    std::pair<Language, ErrorHandling::Error> error_english(Language::ENGLISH, error);
    std::pair<Language, ErrorHandling::Error> error_french(Language::FRENCH, error);

    error_messages_.insert({ error_english, message_english });
    error_messages_.insert({ error_french, message_french });
}

void MessagesManager::add_execution_element(std::string &error_str, Message &message_english,
                           Message &message_french)
{
    auto exec = string_to_exec_type(error_str);

    std::pair<Language, Executor::ExecutionType> error_english(
        Language::ENGLISH, exec);
    std::pair<Language, Executor::ExecutionType> error_french(Language::FRENCH,
                                                              exec);

    execution_messages_.insert({ error_english, message_english });
    execution_messages_.insert({ error_french, message_french });
}

ErrorHandling::Error
MessagesManager::string_to_error(const std::string &task) const
{
    if (task == "command_name")
        return ErrorHandling::Error::COMMAND_ERROR_NAME;
    else if (task == "folder_name")
        return ErrorHandling::Error::FOLDER_ERROR_NAME;
    else if (task == "big_index")
        return ErrorHandling::Error::BIG_INDEX;
    else if (task == "out_of_range")
        return ErrorHandling::Error::OUT_OF_RANGE;
    else if (task == "too_much_arguments")
        return ErrorHandling::Error::TOO_MUCH_ARGUMENTS;
    else if (task == "need_more_arguments")
        return ErrorHandling::Error::NEED_MORE_ARGUMENTS;
    else
        return ErrorHandling::Error::INVALID_INPUT;
}

Executor::ExecutionType
MessagesManager::string_to_exec_type(const std::string &task) const
{
    if (task == "move_folder")
        return Executor::ExecutionType::MOVE_FOLDER;
    else if (task == "back_folder")
        return Executor::ExecutionType::BACK_FOLDER;
    else if (task == "create_command")
        return Executor::ExecutionType::CREATE_COMMAND;
    else if (task == "create_folder")
        return Executor::ExecutionType::CREATE_FOLDER;
    else if (task == "create_combo")
        return Executor::ExecutionType::CREATE_COMBO;
    else if (task == "delete_command")
        return Executor::ExecutionType::DELETE_COMMAND;
    else if (task == "delete_folder")
        return Executor::ExecutionType::DELETE_FOLDER;
    else if (task == "delete_combo")
        return Executor::ExecutionType::DELETE_COMBO;
    else if (task == "reset_folder")
        return Executor::ExecutionType::RESET_FOLDER;
    else if (task == "reset_all")
        return Executor::ExecutionType::RESET_ALL;
    else if (task == "move")
        return Executor::ExecutionType::RESET_ALL;
    else if (task == "display_help")
        return Executor::ExecutionType::DISPLAY_HELP;
    else if (task == "quit")
        return Executor::ExecutionType::QUIT;
    return Executor::ExecutionType::CLEAR;
}

const MessagesManager::error_messages_map &
MessagesManager::get_error_messages() const
{
    return error_messages_;
}

MessagesManager::error_messages_map &MessagesManager::get_error_messages()
{
    return error_messages_;
}

const MessagesManager::execution_messages_map &
MessagesManager::get_execution_messages() const
{
    return execution_messages_;
}

MessagesManager::execution_messages_map &
MessagesManager::get_execution_messages()
{
    return execution_messages_;
}

std::ostream &operator<<(std::ostream &ostr,
                         const MessagesManager::Message &message)
{
    ostr << message.emoji + ' ' + message.message;
    return ostr;
}
