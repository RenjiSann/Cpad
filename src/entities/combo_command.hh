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
        std::vector<std::string> cmds_;

    public:
        /* Ctors */
        ComboCommand();
        ComboCommand(const std::vector<std::string> &cmds);
        virtual ~ComboCommand() = default;

        /* Static tor */
        static ComboPtr from_json(const json &obj_j);

        /* Methods */
        json to_json() const override;

        /* Getters/Setters */
        const std::vector<std::string> &get_commands() const;

        const std::string &to_str() const override;
    };

} // namespace Cpad