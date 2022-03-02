#include <algorithm>
#include <opts.hpp>
#include <stdexcept>

static bool _debug = false;
static std::vector<std::string> _disks;
static std::string _rom;
static int _mem_size;

const bool &program_options::debug()
{
    return _debug;
}

const std::vector<std::string> &program_options::disks()
{
    return _disks;
}

const std::string &program_options::rom_file()
{
    return _rom;
}

const int &program_options::mem_size()
{
    return _mem_size;
}

std::string program_options::get_option(const std::vector<std::string> &args, const std::string &option_name)
{
    for (auto it = args.begin(), end = args.end(); it != end; ++it)
    {
        if (*it == option_name)
        {
            if (it + 1 != end)
            {
                return *(it + 1);
            }
        }
    }

    return "";
}

bool program_options::has_option(const std::vector<std::string> &args, const std::string &option_name)
{
    for (auto it = args.begin(), end = args.end(); it != end; ++it)
    {
        if (*it == option_name)
            return true;
    }

    return false;
}

std::vector<std::string> get_multiple_options(const std::vector<std::string> &args, std::vector<std::string> options)
{
    std::vector<std::string> ret = {};

    for (size_t i = 0; i < args.size(); i++)
    {
        if (std::find(options.begin(), options.end(), args[i]) != options.end())
        {
            ret.push_back(args[++i]);
        }
    }

    return ret;
}

std::string has_options(const std::vector<std::string> &args, std::vector<std::string> options)
{
    for (auto i : options)
    {
        if (program_options::has_option(args, i))
        {
            return program_options::get_option(args, i);
        }
    }

    return "";
}

void program_options::parse(int argc, char **argv)
{
    const std::vector<std::string> args(argv + 1, argv + argc);

    _rom = has_options(args, {"--rom", "--bios", "-r"});

    _disks = get_multiple_options(args, {"-d", "--disk"});

    _debug = has_option(args, "--debug");

    auto mem_size = has_options(args, {"--memory", "-m"});

    _mem_size = mem_size.empty() ? 256 : std::stoi(mem_size);

    if (_rom.empty())
    {
        throw std::runtime_error("Expected bootrom file");
    }

    if (_disks.empty())
    {
        throw std::runtime_error("Expected disk");
    }
}
