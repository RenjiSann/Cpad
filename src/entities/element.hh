#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../json.hh"

namespace Cpad
{
    // Forward declaration of classes.
    class Element;
    class Folder;
    class Command;
    class ComboCommand;
    // Define alias for Element pointers.
    using ElementPtr = std::shared_ptr<Element>;

    /**
     * @brief Describe the common interface to all folder entries.
     */
    class Element
    {
    public:
        /**
         * @brief An enumeration to help dynamic dispatch.
         */
        enum ElementType
        {
            FOLDER,
            COMMAND,
            COMBO_COMMAND
        };

    protected:
        /* parent shared_ptr reference */
        ElementType type_;
        Folder *parent_ = nullptr;

        /**
         * @brief Protected ctor to initialize the `type` field.
         */
        Element(ElementType t);

    public:
        /* Serialization and deserialization method */
        static ElementPtr from_json(const json &obj_j);
        virtual json to_json() const = 0;

        /* Getters/Setters */
        ElementType get_type() const;
        Folder *get_parent() const;
        void set_parent(Folder *folder);

        /**
         * @brief Virtual interface for displaying an element.
         * @return a string reference to print.
         */
        virtual const std::string &to_str() const = 0;
    };

} // namespace Cpad

/*
































*/

class Element
{
public:
    Element(bool is_combo = true);
    Element(std::string name, bool is_folder = true);
    bool get_is_folder() const;
    std::string &get_name();
    bool get_is_combo() const;
    std::vector<Element> &get_combo_elements_();
    void set_name(std::string name);

private:
    bool is_folder_;
    std::string name_;
    bool is_combo_;
    std::vector<Element> combo_elements_;
};