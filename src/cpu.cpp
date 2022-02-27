#include "opts.hpp"
#include <cpu.hpp>

using namespace caar;

void Cpu::do_cycle() {}
void Cpu::trigger_interrupt(int number, bool software)
{
    if (program_options::debug())
    {
        log("Interrupt #{} ({})", number, software ? "software" : "hardware");
    }
}