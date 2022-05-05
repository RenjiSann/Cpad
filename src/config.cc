#include "config.hh"

#include <fstream>
#include <iomanip>
#include <ios>
#include <sstream>

namespace Cpad
{
    Config::Config(const std::string &path)
        : path_(path)
    {
        // Check for file access;
        std::ifstream cfg_file(path);
        json cfg_j;

        // If the file is not open, it does not exist
        if (!cfg_file.is_open())
        {
            // Try to save the default config to the file.
            // If can not open file in output, throw exception.
            std::ofstream file(path);
            if (!file.is_open())
                throw std::ios_base::failure("Cannot access file: " + path);
            // Load the default config in the file.
            file << std::setw(4) << DEFAULT_CONFIG << std::endl;
            file.close();

            // Load the default config to the json.
            cfg_j = json::parse(DEFAULT_CONFIG);
        }
        else
        {
            // In case the config file exist, just load the config.
            cfg_file >> cfg_j;
            cfg_file.close();
        }

        load_json(cfg_j);
    }

    void Config::load_json(const json &obj)
    {
        // Get the emoji status.
        emoji_enabled_ = obj.at(CFG_EMOJI_FIELD).get<bool>();

        // Get the data object for commands.
        auto data = obj.at(CFG_DATA_FIELD);

        hierarchy_ = Folder::from_json(data);
    }

    json Config::to_json() const
    {
        return json{ { CFG_EMOJI_FIELD, emoji_enabled_ },
                     { CFG_DATA_FIELD, hierarchy_->to_json() } };
    }

    void Config::sync_file() const
    {
        // Open the file
        std::ofstream file;

        auto json_cfg = to_json();
        file << std::setw(4) << json_cfg << std::endl;

        file.close();
    }

    const FolderPtr &Config::get_hierarchy() const
    {
        return hierarchy_;
    }

    FolderPtr &Config::get_hierarchy()
    {
        return hierarchy_;
    }

    bool Config::get_emoji_status() const
    {
        return emoji_enabled_;
    }

    void Config::set_emoji_status(bool v)
    {
        emoji_enabled_ = v;
    }

} // namespace Cpad