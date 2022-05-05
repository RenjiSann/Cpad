#pragma once

#include <memory>

#include "element.hh"
#include "json.hh"

namespace Cpad
{
    using CommandPtr = std::shared_ptr<Command>;

    class Command : public Element
    {
    private:
        std::string cmd_;

    public:
        /* Ctors */
        Command() = default;
        Command(const std::string &cmd);

        /* Static ctor */
        static CommandPtr from_json(const json &obj_j);

        /* Methods */
        json to_json() const override;

        /* Getters/Setters */
        const std::string &get_command() const;
    };

} // namespace Cpad