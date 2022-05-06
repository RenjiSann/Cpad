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
    // TODO: Get rid of this.
    auto home_path = std::string(getenv("HOME")) + "/.cpad";
    home_file_check(home_path);

    // Load the configuration.
    auto cfg = Cpad::Config(home_path + ".json");

    // Update emoji status. depending on CLI input.
    std::optional<bool> emoji_cli = check_cli_emoji(argc, argv);
    if (emoji_cli.has_value() && emoji_cli.value() ^ cfg.get_emoji_status())
    {
        cfg.set_emoji_status(emoji_cli.value());
        cfg.sync_file();
    }

    // TODO: Get rid of this.
    auto lines = get_all_lines(home_path);

    // TODO: Get rid of this.
    auto map = Convertor::instance().read(home_path);

    Cpad::Cpad cpad = Cpad::Cpad(cfg);

    // Launch the repl.
    cpad.repl(map, home_path);
}