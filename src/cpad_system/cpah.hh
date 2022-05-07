#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "entities/config.hh"
#include "entities/folder.hh"
#include "executor.hh"
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
         * @brief A wrapping function to handle all the task functions.
         */
        void process_task(const Task &t);

        /**
         * @brief Change the current folder of Cpad.
         */
        void change_current_folder(Folder *f);

        /**
         * @brief Swap the folder's entries at index i and j.
         */
        void swap_entries(int i, int j) const;

        /**
         * @brief Delete the i-th child in the current folder.
         */
        void delete_child(int i) const;

        /**
         * @brief Create a folder in the current folder.
         *
         * @param name: The name of the folder to be created.
         */
        void create_folder(const std::string &name) const;

        /**
         * @brief Create a command in the current folder.
         *
         * @param cmd: The command to be created.
         */
        void create_command(const std::string &cmd) const;

        /**
         * @brief Create a combo in the current folder.
         *
         * @param cmd: The command to be created.
         */
        void create_combo(const std::vector<std::string> &cmds) const;

    public:
        /* Ctors */
        Cpad(Config &);
        Cpad(Cpad &&) = delete;
        Cpad(const Cpad &) = delete;

        /**
         * @brief Launch a read-eval-exe loop.
         */
        void repl();
    };
} // namespace Cpad