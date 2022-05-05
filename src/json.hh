#pragma once

#include <nlohmann/json.hpp>

// Macros for storing JSON keys
#define CFG_EMOJI_FIELD "enable_emojis"
#define CFG_EMOJI_DEFAULT true
#define CFG_DATA_FIELD "data"

#define TYPE_FLAG "type"
#define TYPE_FOLDER "folder"
#define TYPE_COMMAND "command"
#define TYPE_COMBO "combo"

#define FOLDER_NAME "name"
#define FOLDER_CONTENT "content"

#define COMMAND_COMMAND "cmd"

#define COMBO_COMMANDS "cmds"

namespace Cpad
{
    // Alias json to avoid writting nlohmann::json every time.
    using json = nlohmann::json;
}