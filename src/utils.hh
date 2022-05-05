#pragma once

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#define CLI_EMOJI_ON "1"
#define CLI_EMOJI_OFF "0"

/**
 * @brief Return a boolean if the user added an explicit argument to set the
 * emoji state.
 */
std::optional<bool> check_cli_emoji(int argc, char **argv);

/**
 * @brief Check for config file and create it if does not exist.
 *
 * @param home_path : path to the file to check.
 */
void home_file_check(const std::string &home_path);

/**
 * @brief Check emoji enabling in program args or in config.
 */
bool check_emoji(int argc, std::vector<std::string> &lines, char **argv);

bool is_same_emoji_value(std::vector<std::string> &lines, bool value);

/**
 * @brief Update config file's emoji status if different from program arguments.
 */
void rewrite(std::string &path, std::vector<std::string> &, bool display_emoji);

/**
 * @brief Read all the lines of the given file and return a vector of lines.
 *
 * @param home_path : The path to read from.
 * @return : The lines read.
 */
std::vector<std::string> get_all_lines(std::string &home_path);