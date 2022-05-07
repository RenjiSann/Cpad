#include "command.hh"

#include <memory>
#include <string>

#include "element.hh"

namespace Cpad
{
    Command::Command()
        : Element(Element::COMMAND)
    {}

    Command::Command(const std::string &cmd)
        : Element(Element::COMMAND)
        , cmd_(cmd)
    {}

    CommandPtr Command::from_json(const json &obj_j)
    {
        // Assume the "type" is present and correct.
        auto res = Command();
        res.cmd_ = json::string_t(obj_j[COMMAND_COMMAND]);

        return std::make_shared<Command>(std::move(res));
    }

    CommandPtr Command::from_string(const std::string &cmd)
    {
        return std::make_shared<Command>(cmd);
    }

    json Command::to_json() const
    {
        return json{ { TYPE_FLAG, TYPE_COMMAND }, { COMMAND_COMMAND, cmd_ } };
    }

    const std::string &Command::get_command() const
    {
        return cmd_;
    }

    const std::string &Command::to_str() const
    {
        return cmd_;
    }

} // namespace Cpad