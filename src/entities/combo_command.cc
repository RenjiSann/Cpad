#include "combo_command.hh"

#include <memory>
#include <string>

#include "element.hh"

namespace Cpad
{

    ComboCommand::ComboCommand()
        : Element(Element::COMBO_COMMAND)
        , cmds_()
    {}

    ComboCommand::ComboCommand(const std::vector<std::string> &cmds_)
        : Element(Element::COMBO_COMMAND)
        , cmds_(cmds_)
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

    ComboPtr ComboCommand::from_vec(const std::vector<std::string> &cmds)
    {
        return std::make_shared<ComboCommand>(cmds);
    }

    json ComboCommand::to_json() const
    {
        return json{ { TYPE_FLAG, TYPE_COMBO }, { COMBO_COMMANDS, cmds_ } };
    }

    const std::vector<std::string> &ComboCommand::get_commands() const
    {
        return cmds_;
    }

    const std::string &ComboCommand::to_str() const
    {
        return cmds_[0];
    }

} // namespace Cpad