#include "folder.hh"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "element.hh"
#include "json.hh"

namespace Cpad
{
    Folder::Folder()
        : Element(Element::FOLDER)
    {}
    Folder::Folder(const std::string &name)
        : Element(Element::FOLDER)
        , name_(name)
        , children_()
    {}

    FolderPtr Folder::from_json(const json &obj_j)
    {
        // Assume this is a folder json.
        auto res = Folder();

        res.name_ = json::string_t(obj_j[FOLDER_NAME]);
        auto children = json::array_t(obj_j[FOLDER_CONTENT]);

        res.children_.reserve(children.size());
        // Parse every JSON child into deserialized form.
        for (const auto &c : children)
            res.children_.push_back(Element::from_json(c));

        return std::make_shared<Folder>(std::move(res));
    }

    FolderPtr Folder::empty(const std::string &name)
    {
        return std::make_shared<Folder>(Folder(name));
    }

    json Folder::to_json() const
    {
        std::vector<json> json_children;
        json_children.reserve(children_.size());
        for (const auto &c : children_)
            json_children.push_back(c->to_json());

        return json{ { TYPE_FLAG, TYPE_FOLDER },
                     { FOLDER_NAME, name_ },
                     { FOLDER_CONTENT, json_children } };
    }

    const std::string &Folder::get_name() const
    {
        return name_;
    }

    const std::vector<ElementPtr> &Folder::get_children() const
    {
        return children_;
    }

    std::vector<ElementPtr> &Folder::get_children()
    {
        return children_;
    }

    void Folder::add_child(ElementPtr child)
    {
        child->set_parent(this);
        children_.push_back(child);
    }

    void Folder::remove_child(int index)
    {
        children_.erase(children_.begin() + index);
    }

    void Folder::clear_children()
    {
        children_.clear();
    }

    void Folder::swap_children(int i, int j)
    {
        if (i != j)
            std::swap(children_[i], children_[j]);
    }

    const std::string &Folder::to_str() const
    {
        return name_;
    }

} // namespace Cpad

/*








*/

Folder::Folder(std::vector<Element> &elements)
    : elements_(elements)
{}

void Folder::add_command(std::string &command_name)
{
    elements_.push_back(Element(command_name, false));
}

void Folder::add_folder(std::string &folder_name)
{
    elements_.push_back(Element(folder_name, true));
}

std::vector<Element> &Folder::get_elements()
{
    return elements_;
}