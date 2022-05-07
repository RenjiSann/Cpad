#pragma once

#include <cstddef>
#include <map>
#include <string>

#include "entities/config.hh"
#include "entities/folder.hh"
namespace Cpad
{

    class Cpad
    {
    private:
        /// Config holding the emoji allowance status, and file hierarchy.
        Config *cfg_;
        /// Pointer to the current working directory.
        Folder *cur_folder_;

        /**
         * @brief Swap the folder's entries at index i and j.
         */
        void swap_entries(size_t i, size_t j) const;

        /**
         * @brief Add a folder in the current directory.
         *
         * @param name: The name of the folder to be created.
         */
        void add_folder(const std::string &name) const;

    public:
        /* Ctors */
        Cpad(Config &);
        Cpad(Cpad &&) = delete;
        Cpad(const Cpad &) = delete;

        /**
         * @brief Launch a read-eval-exe loop.
         */
        void repl(std::map<std::string, ::Folder> &map,
                  const std::string &home_path);
    };
} // namespace Cpad