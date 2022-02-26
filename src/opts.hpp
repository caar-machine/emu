#pragma once
#include <string>
#include <vector>

namespace program_options
{

const std::string &rom_file();
const std::vector<std::string> &disks();
const bool &debug();
const int &mem_size();

void parse(int argc, char **argv);

bool has_option(const std::vector<std::string> &args, const std::string &option_name);

std::string get_option(const std::vector<std::string> &args, const std::string &option_name);

} // namespace program_options