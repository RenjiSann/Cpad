#include "command.hh"

#include "element.hh"

namespace Cpad
{
    Command::Command(const std::string &cmd)
        : cmd_(cmd)
    {}

    CommandPtr Command::from_json(const json &obj_j)
    {
        // Assume the "type" is present and correct.
        auto res = Command();
        res.cmd_ = json::string_t(obj_j[COMMAND_COMMAND]);

        return std::make_shared<Command>(std::move(res));
    }

    json Command::to_json() const
    {
        return json{ { TYPE_FLAG, TYPE_COMMAND }, { COMMAND_COMMAND, cmd_ } };
    }

    const std::string &Command::get_command() const
    {
        return cmd_;
    }
} // namespace Cpad