#include "combo_command.hh"

#include "element.hh"

namespace Cpad
{

    ComboCommand::ComboCommand(const std::vector<std::string> &cmds_)
        : cmds_(cmds_)
    {}

    ComboPtr ComboCommand::from_json(const json &obj_j)
    {
        // Assume the type is present and is correct.
        auto res = ComboCommand();

        auto json_cmds = json::array_t(obj_j[COMBO_COMMANDS]);

        res.cmds_.reserve(json_cmds.size());
        for (const auto &js : json_cmds)
            res.cmds_.push_back(json::string_t(js));

        return std::make_shared<ComboCommand>(std::move(res));
    }

    json ComboCommand::to_json() const
    {
        return json{ { TYPE_FLAG, TYPE_COMBO }, { COMBO_COMMANDS, cmds_ } };
    }

    const std::vector<std::string> &ComboCommand::get_commands() const
    {
        return cmds_;
    }

} // namespace Cpad