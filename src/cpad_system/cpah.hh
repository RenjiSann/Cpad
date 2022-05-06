#pragma once

#include <map>
#include <string>

#include "entities/config.hh"
#include "entities/folder.hh"
namespace Cpad
{

    class Cpad
    {
    private:
        Config *cfg_;
        Folder *current_;

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