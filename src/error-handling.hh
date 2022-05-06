#pragma once

#include <map>

#include "entities/folder.hh"
#include "scaner.hh"
#include "singleton.hh"

class ErrorHandling : public Singleton<ErrorHandling>
{
public:
    enum class Error
    {
        COMMAND_ERROR_NAME,
        FOLDER_ERROR_NAME,
        BIG_INDEX,
        OUT_OF_RANGE,
        TOO_MUCH_ARGUMENTS,
        NEED_MORE_ARGUMENTS,
        INVALID_INPUT,
        NONE
    };

    /// Verify tokens' validity and return correspoding error if not.
    Error check_error(const Cpad::Folder *folder, Tokens &tokens);

private:
    /// Cas argument in int and check bounds for current folder.
    Error parsing_int(const Cpad::Folder *folder, std::string &input);

    Error check_arguments_size(Tokens &tokens);

    Error check_execution(const Cpad::Folder *folder, Tokens &tokens);

    /// Check if a command item can be created, without creating a duplicate.
    Error check_create_command(const Cpad::Folder *folder, Tokens &tokens);

    /// Check if a folder item can be created, without creating a duplicate.
    Error check_create_folder(const Cpad::Folder *folder, Tokens &tokens);

    /// Check if a command combo item can be created, without creating a
    /// duplicate.
    Error check_create_combo(Tokens &tokens);

    /// Check that given arguments are correct indexes.
    Error check_move(const Cpad::Folder *folder, Tokens &tokens);

    // TODO
    /// This one I dont know.
    Error check_reset_folder(const Cpad::Folder *folder, Tokens &tokens);

    Error check_reset_all();

    /// Verify that given arguments are only numbers and check bounds
    Error check_delete(const Cpad::Folder *folder, Tokens &tokens);
};

/**
 * @brief Return a string corresponding to the what of the error.
 */
const std::string &error_to_str(ErrorHandling::Error e);