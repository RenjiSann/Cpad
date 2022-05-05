#pragma once

#include <memory>
#include <string>
#include <vector>

#include "element.hh"
#include "json.hh"

namespace Cpad
{
    using ComboPtr = std::shared_ptr<ComboCommand>;

    class ComboCommand: public Element
    {
    private:
        std::vector<std::string> cmds_;

    public:
        /* Ctors */
        ComboCommand() = default;
        ComboCommand(const std::vector<std::string> &cmds_);

        /* Static tor */
        static ComboPtr from_json(const json &obj_j);

        /* Methods */
        json to_json() const;

        /* Getters/Setters */
        const std::vector<std::string> &get_commands() const;
    };

} // namespace Cpad