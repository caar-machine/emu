#pragma once
#include <devices/bus.hpp>
#include <ram.hpp>

#define CPU_R0 0
#define CPU_R1 1
#define CPU_R2 2
#define CPU_R3 3
#define CPU_R4 4
#define CPU_R5 5
#define CPU_R6 6
#define CPU_R7 7
#define CPU_SP 8
#define CPU_PC 9
#define CPU_IVT 10
#define CPU_PT 11
#define CPU_PF 12

namespace caar
{

struct [[gnu::packed]] InstructionEncoding
{

    uint8_t value : 6;
    uint8_t param_type : 2;
};

struct InstructionDecoding
{
    enum
    {
        DECODE_IMM,
        DECODE_REG,
    } type;
    union
    {
        uint32_t *ptr;
        uint32_t val;
    };
};

class Cpu
{
public:
    Cpu(Ram &ram, Bus &bus) : _ram(ram), _bus(bus)
    {
        regs[CPU_PC] = 0x1000;
        regs[CPU_SP] = MEMORY_SIZE;
        flags.EQ = 0;
        flags.LT = 0;
        flags.PL = 0;
    }

    void do_cycle();
    void trigger_interrupt(int number, bool software = true);

    uint32_t regs[14] = {0};

private:
    Ram &_ram;
    Bus &_bus;

    struct
    {
        uint8_t EQ : 1;
        uint8_t LT : 1;
        uint8_t PL : 1;
    } flags;

    uint8_t fetch();

    void push(uint32_t what);

    uint32_t pop();

    InstructionDecoding decode(uint8_t byte, bool set = false);
};

} // namespace caar
