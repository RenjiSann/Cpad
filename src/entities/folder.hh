#pragma once

#include <memory>
#include <string>
#include <vector>

#include "element.hh"
#include "json.hh"

namespace Cpad
{
    using FolderPtr = std::shared_ptr<Folder>;

    class Folder : public Element
    {
    private:
        std::string name_;
        std::vector<ElementPtr> children_;

    public:
        /* Ctors */
        Folder() = default;
        Folder(const std::string &name);

        /* Static ctors */
        /// JSON deserialization ctor.
        static FolderPtr from_json(const json &obj_j);
        /// Empty folder ctor.
        static FolderPtr empty(const std::string &name);

        /* Methods */
        /* Serialization method */
        json to_json() const override;
        /* Override the "is_folder" getter. */
        bool is_folder() const override;

        /**
         * @brief Swap the children at index i and j.
         */
        void swap_children(int i, int j);

        /* Getters/Setters */
        const std::string &get_name() const;
        const std::vector<ElementPtr> &get_children() const;
        std::vector<ElementPtr> &get_children();
        void add_child(ElementPtr child);
        void remove_child(int index);
        void clear_children();
    };

} // namespace Cpad

/*

























*/
class Folder
{
public:
    Folder() = default;
    Folder(std::vector<Element> &elements);
    void add_command(std::string &command_name);
    void add_folder(std::string &folder_name);

    friend std::ostream &operator<<(std::ostream &ostr, Folder &folder);

    std::vector<Element> &get_elements();

private:
    std::vector<Element> elements_;
};

std::ostream &operator<<(std::ostream &ostr, const Folder &folder);