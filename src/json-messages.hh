#pragma once

#include <nlohmann/json.hpp>

/// Macro for enabling pretty printing of JSON.
#define PRETTY_CONFIG

// Macros for storing JSON keys
#define ERRORS_FIELD "errors"
#define EXECUTION_FIELD "execution"
#define EMOJI_FIELD "emoji"
#define MESSAGES_FIELD "messages"
#define LANGUAGE_FIELD "language"

#define ENGLISH_FIELD "english"
#define FRENCH_FIELD "french"

// fields corresponding to the possible errors obtained

#define ERROR_COMMAND_NAME "command_name"
#define ERROR_FOLDER_NAME "folder_name"
#define ERROR_BIG_INDEX "big_index"
#define ERROR_OUT_OF_RANGE "out_of_range"
#define ERROR_TOO_MUCH_ARGUMENTS "too_much_arguments"
#define ERROR_NEED_MORE_ARGUMENTS "need_more_arguments"
#define ERROR_INVALID_INPUT "invalid_input"

// fields corresponding to the possible execution obtained
// TODO

#define EXEC_MOVE_FOLDER "move_folder"
#define EXEC_BACK_FOLDER "back_folder"
#define EXEC_CREATE_COMMAND "create_command"
#define EXEC_CREATE_FOLDER "create_folder"
#define EXEC_CREATE_COMBO "create_combo"
#define EXEC_DELETE_COMMAND "delete_command"
#define EXEC_DELETE_FOLDER "delete_folder"
#define EXEC_DELETE_COMBO "delete_combo"
#define EXEC_RESET_FOLDER "reset_folder"
#define EXEC_RESET_ALL "reset_all"
#define EXEC_MOVE "move"
#define EXEC_DISPLAY_HELP "display_help"
#define EXEC_CLEAR "clear"
#define EXEC_QUIT "quit"
