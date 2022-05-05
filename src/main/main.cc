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
#include "cpad/cpah.hh"
#include "display.hh"
#include "entities/config.hh"
#include "executor.hh"
#include "scaner.hh"
#include "utils.hh"

int main(int argc, char **argv)
{
    auto home_path = std::string(getenv("HOME")) + "/.cpad";
    home_file_check(home_path);

    auto cfg = Cpad::Config(home_path + ".json");

    // Update emoji status. depending on CLI input.
    std::optional<bool> emoji_cli = check_cli_emoji(argc, argv);
    if (emoji_cli.has_value() && emoji_cli.value() ^ cfg.get_emoji_status())
    {
        cfg.set_emoji_status(emoji_cli.value());
        cfg.sync_file();
    }

    auto lines = get_all_lines(home_path);
    bool display_emoji = check_emoji(argc, lines, argv);

    rewrite(home_path, lines, display_emoji);
    auto map = Convertor::instance().read(home_path, display_emoji);

    Cpad::Cpad system = Cpad::Cpad(cfg);

    system.repl(map, home_path);
}