#include "opts.hpp"
#include <cpu.hpp>

using namespace caar;

void Cpu::do_cycle() {}
void Cpu::trigger_interrupt(int number, bool software)
{
    if (program_options::debug())
    {
        log("Interrupt #{} ({})", number, software ? "software" : "hardware");
        log("registers:");
        log("r0={:x} r1={:x} r2={:x}", regs[CPU_R0], regs[CPU_R1], regs[CPU_R2]);
        log("r3={:x} r4={:x} r5={:x}", regs[CPU_R3], regs[CPU_R4], regs[CPU_R5]);
        log("r6={:x} r7={:x} pc={:x}", regs[CPU_R6], regs[CPU_R7], regs[CPU_PC]);
        log("sp={:x} pt={:x} pf={:x}", regs[CPU_SP], regs[CPU_PT], regs[CPU_PF]);
        log("ivt={:x}", regs[CPU_IVT]);
    }
}