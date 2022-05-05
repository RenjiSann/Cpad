#pragma once

#include <memory>
#include <string>
#include <vector>

#include "element.hh"
#include "json.hh"

namespace Cpad
{
    using ComboPtr = std::shared_ptr<ComboCommand>;

    class ComboCommand : public Element
    {
    private:
        std::string name_;
        std::vector<std::string> cmds_;

    public:
        /* Ctors */
        ComboCommand(const std::string &name);
        ComboCommand(const std::string &name,
                     const std::vector<std::string> &cmds);

        /* Static tor */
        static ComboPtr from_json(const json &obj_j);

        /* Methods */
        json to_json() const override;

        /* Getters/Setters */
        const std::vector<std::string> &get_commands() const;

        const std::string& to_str() const override;
    };

} // namespace Cpad