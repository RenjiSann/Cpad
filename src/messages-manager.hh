#pragma once

#include <map>

#include "error-handling.hh"
#include "executor.hh"
#include "json-messages.hh"
#include "json.hh"
#include "singleton.hh"
#include <iostream>

/**
 * @brief Class responsible for json parsing of resource messages, and their
 * storage.
 */

class MessagesManager : public Singleton<MessagesManager>
{
public:
    /**
     * @brief An enumeration to choose which language should be displayed.
     */
    enum class Language
    {
        ENGLISH,
        FRENCH
    };

    struct Message
    {
        Message(std::string json_emoji, std::string json_message)
            : emoji(json_emoji)
            , message(json_message)
        {}
        std::string emoji;
        std::string message;
    };

    using error_messages_map =
        std::map<std::pair<Language, ErrorHandling::Error>, Message>;
    using execution_messages_map =
        std::map<std::pair<Language, Executor::ExecutionType>, Message>;

    nlohmann::json read_json(const std::string &path);
    void from_json(const nlohmann::json &obj_j);

    const error_messages_map &get_error_messages() const;
    error_messages_map &get_error_messages();

    const execution_messages_map &get_execution_messages() const;
    execution_messages_map &get_execution_messages();

private:
    ErrorHandling::Error string_to_error(const std::string &task) const;
    Executor::ExecutionType string_to_exec_type(const std::string &task) const;
    void add_error_element(std::string& error, Message& message_english, Message& message_french);
    void add_execution_element(std::string& error, Message& message_english, Message& message_french);

    void parse_messages(const nlohmann::json &obj_j, bool is_error);


private:
    error_messages_map error_messages_;
    execution_messages_map execution_messages_;
};

std::ostream &operator<<(std::ostream &ostr,
                         const MessagesManager::Message &message);
