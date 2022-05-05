#pragma once

#include <memory>
#include <string>
#include <vector>

#include "json.hh"

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
    protected:
        /* parent shared_ptr reference */
        Folder *parent_ = nullptr;

    public:
        /**
         * @brief Default value return false and is overriden by Folder.
         */
        virtual bool is_folder() const;

        /* Serialization and deserialization method */
        static ElementPtr from_json(const json &obj_j);
        virtual json to_json() const = 0;

        /* Getters/Setters */
        Folder *get_parent() const;
        void set_parent(Folder *folder);
    };

} // namespace Cpad

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