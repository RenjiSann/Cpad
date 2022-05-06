#include "error-handling.hh"

#include <algorithm>
#include <iostream>
#include <string>
#include <unistd.h>

#include "entities/command.hh"
#include "entities/element.hh"
#include "entities/folder.hh"

ErrorHandling::Error ErrorHandling::parsing_int(const Cpad::Folder *folder,
                                                std::string &input)
{
    int value;
    try
    {
        value = std::stoi(input) - 1;
    }
    catch (...)
    {
        return Error::BIG_INDEX;
    }
    size_t value_t = static_cast<size_t>(value);
    if (value < 0 || value_t >= folder->get_children().size())
        return Error::OUT_OF_RANGE;
    return Error::NONE;
}

ErrorHandling::Error ErrorHandling::check_arguments_size(Tokens &tokens)
{
    int arguments_expected =
        ArgumentType::instance().number_arguments_expected(tokens.first);
    int current_size = tokens.second.size();
    if (current_size > arguments_expected)
        return Error::TOO_MUCH_ARGUMENTS;
    else if (current_size < arguments_expected)
        return Error::NEED_MORE_ARGUMENTS;
    return Error::NONE;
}

ErrorHandling::Error ErrorHandling::check_error(const Cpad::Folder *folder,
                                                Tokens &tokens)
{
    auto error_arguments_size = check_arguments_size(tokens);
    if (error_arguments_size != Error::NONE
        && tokens.first != ArgumentType::ELEMENT_TYPE::CREATE_COMBO)
        return error_arguments_size;

    switch (tokens.first)
    {
    case ArgumentType::ELEMENT_TYPE::EXECUTION:
        return check_execution(folder, tokens);
    case ArgumentType::ELEMENT_TYPE::CREATE_COMMAND:
        return check_create_command(folder, tokens);
    case ArgumentType::ELEMENT_TYPE::CREATE_FOLDER:
        return check_create_folder(folder, tokens);
    case ArgumentType::ELEMENT_TYPE::DELETE:
        return check_delete(folder, tokens);
    case ArgumentType::ELEMENT_TYPE::MOVE:
        return check_move(folder, tokens);
    case ArgumentType::ELEMENT_TYPE::RESET_FOLDER:
        return check_reset_folder(folder, tokens);
    case ArgumentType::ELEMENT_TYPE::RESET_ALL:
        return check_reset_all();
    case ArgumentType::ELEMENT_TYPE::CREATE_COMBO:
        return check_create_combo(tokens);
    }

    return Error::INVALID_INPUT;
}

ErrorHandling::Error ErrorHandling::check_execution(const Cpad::Folder *folder,
                                                    Tokens &tokens)
{
    std::string argument = tokens.second[0];

    bool is_number = std::all_of(argument.cbegin(), argument.cend(), ::isdigit);

    bool is_valid_command = (folder->get_parent() != nullptr && argument == "b")
        || argument == "h" || argument == "q";

    if (!is_number && !is_valid_command)
        return Error::INVALID_INPUT;

    if (is_number)
        return parsing_int(folder, argument);
    return Error::NONE;
}

ErrorHandling::Error
ErrorHandling::check_create_command(const Cpad::Folder *folder, Tokens &tokens)
{
    std::string argument = tokens.second[1];

    // Check all children.
    for (const auto &elm : folder->get_children())
    {
        if (elm->get_type() != Cpad::Element::COMMAND)
            continue;

        auto cmd = dynamic_cast<Cpad::Command &>(*elm);

        if (cmd.get_command() == argument)
            return Error::COMMAND_ERROR_NAME;
    }
    return Error::NONE;
}

ErrorHandling::Error
ErrorHandling::check_create_folder(const Cpad::Folder *folder, Tokens &tokens)
{
    std::string argument = tokens.second[1];

    for (const auto &elm : folder->get_children())
    {
        if (elm->get_type() != Cpad::Element::FOLDER)
            continue;

        auto fold = dynamic_cast<Cpad::Folder &>(*elm);
        if (fold.get_name() == argument)
            return Error::FOLDER_ERROR_NAME;
    }
    return Error::NONE;
}

ErrorHandling::Error ErrorHandling::check_create_combo(Tokens &tokens)
{
    if (tokens.second.size() < 2)
        return Error::NEED_MORE_ARGUMENTS;
    auto elem = tokens.second[1];
    if (elem.size() == 2 && elem[0] == '{' && elem[1] == '}')
        return Error::INVALID_INPUT;
    if (elem.size() >= 3 && elem[0] == '{' && elem[elem.size() - 1] == '}')
        if (tokens.second.size() < 3)
            return Error::NEED_MORE_ARGUMENTS;
    return Error::NONE;
}

ErrorHandling::Error ErrorHandling::check_move(const Cpad::Folder *folder,
                                               Tokens &tokens)
{
    std::string src_index = tokens.second[1];
    std::string dst_index = tokens.second[2];

    if (!std::all_of(src_index.begin(), src_index.end(), ::isdigit)
        || !std::all_of(dst_index.begin(), dst_index.end(), ::isdigit))
        return Error::INVALID_INPUT;

    auto error_parsing_src = parsing_int(folder, src_index);

    if (error_parsing_src != Error::NONE)
        return error_parsing_src;

    return parsing_int(folder, dst_index);
}

ErrorHandling::Error
ErrorHandling::check_reset_folder(const Cpad::Folder *folder, Tokens &tokens)
{
    std::string argument = tokens.second[1];
    if (!std::all_of(argument.cbegin(), argument.cend(), ::isdigit))
        return Error::INVALID_INPUT;

    return parsing_int(folder, argument);
}

ErrorHandling::Error ErrorHandling::check_reset_all()
{
    return Error::NONE;
}

ErrorHandling::Error ErrorHandling::check_delete(const Cpad::Folder *folder,
                                                 Tokens &tokens)
{
    std::string argument = tokens.second[1];

    if (!std::all_of(argument.cbegin(), argument.cend(), ::isdigit))
        return Error::INVALID_INPUT;

    return parsing_int(folder, argument);
}

const std::string &error_to_str(ErrorHandling::Error e)
{
    static const std::string strs[] = {
        std::string(
            "You can't have the same commands in the same folder twice."),
        std::string("You can't have two folders with the same name in the "
                    "whole project."),
        std::string("Your index is too big."),
        std::string("Your arguments must be in the interval of your folder."),
        std::string("Your function needs more arguments to be executed."),
        std::string("Your function needs less arguments to be executed."),
        std::string("Your input is invalid"),
        std::string("No error")
    };

    return strs[static_cast<unsigned int>(e)];
}