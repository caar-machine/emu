#include <algorithm>
#include <cpu.hpp>
#include <devices/gpu.hpp>
#include <fstream>
#include <log.hpp>
#include <opts.hpp>

#ifndef GIT_HASH
#    define GIT_HASH "unknown"
#endif

constexpr int instructions_per_sec = 25'00'00'00;

int main(int argc, char **argv)
{
    try
    {
        program_options::parse(argc, argv);
    }

    catch (const std::exception &e)
    {
        error("{}", e.what());
        error("Usage: {} [\033[1m--rom|--bios|-r\033[0m bios_file] [--debug] [\033[1m--disk\033[0m disk_file] [\033[1m-m|--memory\033[0m mem_size]", argv[0]);
        return EXIT_FAILURE;
    }

    log("Welcome to caar-emu @ {} !", GIT_HASH);

    if (program_options::mem_size() < 8)
    {
        log("Memory size under 8 mb is not recommended, the system might not work properly");
    }

    log("Starting VM with the following parameters:");
    log("  - memory size: {}mb", program_options::mem_size());
    log("  - bootrom path: {}", program_options::rom_file());
    log("  - disks: [{}]", fmt::join(program_options::disks(), ", "));

    caar::Ram ram(program_options::mem_size());

    caar::Bus bus(ram);

    caar::dev::Gpu gpu(FB_WIDTH, FB_HEIGHT);

    bus.attach(gpu);

    caar::Cpu cpu(ram, bus);

    std::ifstream input(program_options::rom_file(), std::ios::binary);

    std::vector<char> buffer(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));

    input.close();

    ram.write_chunk(buffer, 0);

    SDL_Event event;

    bool done = false;

    int tick_start = SDL_GetTicks();
    int tick_end = SDL_GetTicks();
    int ticks_per_second = 60;
    int ticks = 0;

    while (!done)
    {
        int ms = std::max<int>(SDL_GetTicks() - tick_start, 1);
        int instr_to_run = instructions_per_sec / ticks_per_second / ms;

        tick_start = SDL_GetTicks();

        for (int i = 0; i < ms; i++)
        {
            for (int j = 0; j < instr_to_run; j++)
            {
                cpu.do_cycle();
            }
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = true;
            }

            if (event.type == SDL_KEYDOWN)
            {
                cpu.trigger_interrupt(10, false);
            }
        }

        if (ticks % ticks_per_second == 0)
        {
            gpu.update();
        }

        tick_end = SDL_GetTicks();

        int time_left = 1000 / ticks_per_second - (int)(tick_end - tick_start);

        if (time_left > 0)
        {
            SDL_Delay(time_left);
        }
    }

    return 0;
}
