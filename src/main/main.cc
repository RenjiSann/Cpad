#include <algorithm>
#include <cstddef>
#include <cstring>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <unistd.h>

#include "argument-type.hh"
#include "convertor.hh"
#include "cpad_system/cpah.hh"
#include "display.hh"
#include "entities/config.hh"
#include "executor.hh"
#include "scaner.hh"
#include "utils.hh"

int main(int argc, char **argv)
{
    auto home_path = std::string(getenv("HOME")) + "/.cpad";

    // Load the configuration.
    auto cfg = Cpad::Config(home_path);

    // Update emoji status depending on CLI input.
    std::optional<bool> emoji_cli = check_cli_emoji(argc, argv);
    if (emoji_cli.has_value() && emoji_cli.value() ^ cfg.get_emoji_status())
    {
        cfg.set_emoji_status(emoji_cli.value());
        cfg.sync_file();
    }

    Cpad::Cpad cpad = Cpad::Cpad(cfg);

    // Launch the repl.
    cpad.repl();
}