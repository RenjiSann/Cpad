#pragma once

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * @brief Check for config file and create it if does not exist.
 * 
 * @param home_path : path to the file to check.
 */
void home_file_check(const std::string &home_path);

bool check_arguments(int argc, std::vector<std::string> &lines, char **argv);

bool is_same_emoji_value(std::vector<std::string> &lines, bool value);

void rewrite(std::string &path, std::vector<std::string> &, bool display_emoji);

std::vector<std::string> get_all_lines(std::string &home_path);