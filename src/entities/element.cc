#include "element.hh"

#include <memory>
#include <stdexcept>
#include <string>

#include "combo_command.hh"
#include "command.hh"
#include "folder.hh"

namespace Cpad
{
    Element::Element(ElementType t)
        : type_(t)
    {}

    ElementPtr Element::from_json(const json &obj_j)
    {
        // Check if the type flag is present
        if (!obj_j.contains(TYPE_FLAG))
            throw std::invalid_argument("Invalid json");

        auto type = obj_j.at(TYPE_FLAG).get<std::string>();

        if (type == TYPE_FOLDER)
            return Folder::from_json(obj_j);
        if (type == TYPE_COMMAND)
            return Command::from_json(obj_j);
        if (type == TYPE_COMBO)
            return ComboCommand::from_json(obj_j);
        throw std::invalid_argument("Invalid type");
    }

    Element::ElementType Element::get_type() const
    {
        return type_;
    }

    Folder *Element::get_parent() const
    {
        return parent_;
    }

    void Element::set_parent(Folder *folder)
    {
        parent_ = folder;
    }

} // namespace Cpad

/*



























*/

Element::Element(std::string name, bool is_folder)
    : is_folder_(is_folder)
    , name_(name)
    , is_combo_(false)
{}

Element::Element(bool is_combo)
    : is_folder_(false)
    , name_("")
    , is_combo_(is_combo)
{}

bool Element::get_is_folder() const
{
    return is_folder_;
}

std::string &Element::get_name()
{
    return name_;
}

bool Element::get_is_combo() const
{
    return is_combo_;
}

std::vector<Element> &Element::get_combo_elements_()
{
    return combo_elements_;
}

void Element::set_name(std::string name)
{
    name_ = name;
}