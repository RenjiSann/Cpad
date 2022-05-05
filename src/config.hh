#pragma once

#include <map>
#include <string>

#include "folder.hh"
#include "json.hh"

#define PRETTY_CONFIG

#define DEFAULT_CONFIG                                                         \
    "{\n"                                                                      \
    "  \"enable_emojis\": true,\n"                                             \
    "  \"current_dir\": \"sub_dir_1\",\n"                                      \
    "  \"data\": {\n"                                                          \
    "      \"index\": 0,\n"                                                    \
    "      \"type\": \"folder\",\n"                                            \
    "      \"name\": \".\",\n"                                                 \
    "      \"content\": []\n"                                                  \
    "   }\n"                                                                   \
    "}"

namespace Cpad
{

    /**
     * @brief A class holding the data of Cpad.
     *
     */
    class Config
    {
    private:
        std::string path_; ///< The path of the config file.
        bool emoji_enabled_; ///< Emoji enabling status.
        FolderPtr hierarchy_; ///< All folders of the config.

        void load_json(const json &obj_j);
        json to_json() const;

    public:
        /* Constructors */
        Config(const std::string &path);
        Config(Config &&) = delete;
        Config(const Config &) = delete;

        /* Methods */
        /**
         * @brief Save the current configuration to the file.
         */
        void sync_file() const;

        /* Getters/Setters */
        const FolderPtr &get_hierarchy() const;
        FolderPtr &get_hierarchy();
        bool get_emoji_status() const;

        void set_emoji_status(bool v);
    };
} // namespace Cpad