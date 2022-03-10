#include "opts.hpp"
#include <cpu.hpp>
#include <devices/io.hpp>

using namespace caar;

#define CPU_OP(OP)                             \
    uint32_t *ptr = decode(fetch(), true).ptr; \
    uint32_t rhs = decode(fetch(), false).val; \
    *ptr = *ptr OP rhs;

#define CPU_SOP(OP)                            \
    uint32_t *ptr = decode(fetch(), true).ptr; \
    uint32_t rhs = decode(fetch(), false).val; \
    *ptr = OP rhs;

// TODO: cell operations
#define CPU_CELL_OP(TYPE)                      \
    uint32_t lhs = decode(fetch(), true).val;  \
    uint32_t rhs = decode(fetch(), false).val; \
    (void)lhs;                                 \
    (void)rhs;
// ram_write(lhs + offsetof(Cons, TYPE), MEM_4_BYTES, rhs, cpu->ram);

#define CPU_GET_LHS_AND_RHS()                  \
    uint32_t lhs = decode(fetch(), false).val; \
    uint32_t rhs = decode(fetch(), false).val;

#define CPU_SET_LHS_AND_GET_RHS()              \
    uint32_t *lhs = decode(fetch(), true).ptr; \
    uint32_t rhs = decode(fetch(), false).val;

#define CPU_GET_VAL() \
    uint32_t val = decode(fetch(), false).val;

#define CPU_SET_VAL() \
    uint32_t *val = decode(fetch(), true).ptr;

#define TO_16(a, b) \
    a | (b << 8)

#define TO_32(a, b, c, d) \
    a | (b << 8) | (c << 16) | (d << 24)

inline uint8_t Cpu::fetch()
{
    return _ram.read<uint8_t>(regs[CPU_PC]++);
}

void Cpu::push(uint32_t what)
{

    if (regs[CPU_SP] == (MEMORY_SIZE - STACK_SIZE))
    {
        error("stack overflow at PC {:x}", regs[CPU_PC]);
    }

    if (what > 255)
    {
        _ram.write<uint32_t>(regs[CPU_SP] - 4, what);
    }

    else
    {
        _ram.write<uint8_t>(regs[CPU_SP] - 4, what);
    }

    regs[CPU_SP] -= 4;
}

uint32_t Cpu::pop()
{
    if (regs[CPU_SP] >= MEMORY_SIZE)
    {
        error("pop() out of bounds at PC {:x}", regs[CPU_PC]);
        return 0;
    }

    auto ret = _ram.read<uint32_t>(regs[CPU_SP]);

    regs[CPU_SP] += 4;

    return ret;
}

InstructionDecoding Cpu::decode(uint8_t byte, bool set)
{
    InstructionDecoding ret = {};

    const InstructionEncoding encoded = *(InstructionEncoding *)&byte;

    if (encoded.param_type == 0)
    {
        ret.type = set ? InstructionDecoding::DECODE_REG : InstructionDecoding::DECODE_IMM;

        if (set)
        {
            ret.ptr = &regs[encoded.value];
        }

        else
        {
            ret.val = regs[encoded.value];
        }
    }

    else if (encoded.param_type == 0b10)
    {
        ret.type = InstructionDecoding::DECODE_IMM;

        switch (encoded.value)
        {
        case 0:
            ret.val = fetch();
            break;

        case 1:
            ret.val = TO_16(fetch(), fetch());
            break;

        case 2:
            ret.val = TO_32(fetch(), fetch(), fetch(), fetch());
            break;
        case 3:
            ret.val = _ram.read<uint64_t>(regs[CPU_PC] += 8);
            break;
        }
    }

    return ret;
}

void Cpu::do_cycle()
{
    void *array[] = {
        [0] = &&cons,
        [1] = &&car,
        [2] = &&cdr,
        [3] = &&nop,
        [4] = &&ldr,
        [5] = &&str,
        [6] = &&add,
        [7] = &&sub,
        [8] = &&div,
        [9] = &&mul,
        [0xA] = &&mod,
        [0xB] = &&_not,
        [0xC] = &&_and,
        [0xD] = &&_or,
        [0xE] = &&_xor,
        [0xF] = &&_push,
        [0x10] = &&_pop,
        [0x11] = &&jmp,
        [0x12] = &&cmp,
        [0x13] = &&je,
        [0x14] = &&jne,
        [0x15] = &&jlt,
        [0x16] = &&jgt,
        [0x17] = &&in,
        [0x18] = &&out,
        [0x19] = &&stw,
        [0x1a] = &&ldw,
        [0x1b] = &&_int,
        [0x1c] = &&call,
        [0x1d] = &&mov,
    };

    uint8_t byte = fetch();

    if (byte > 0x1D)
    {
        log("Invalid opcode: {:x}", byte);
    }

    goto *array[byte];

cons:
{
    log("TODO: cons at pc {:x}", regs[CPU_PC]);
    exit(0);
    return;
};

car:
{
    log("TODO: car");
    exit(0);
    return;
};

cdr:
{
    log("TODO: cdr");
    exit(0);
    return;
};

nop:
{
    if (program_options::debug())
    {
        log("nop at PC {:x}", regs[CPU_PC]);
        log("registers:");
        log("r0={:x} r1={:x} r2={:x}", regs[CPU_R0], regs[CPU_R1], regs[CPU_R2]);
        log("r3={:x} r4={:x} r5={:x}", regs[CPU_R3], regs[CPU_R4], regs[CPU_R5]);
        log("r6={:x} r7={:x} pc={:x}", regs[CPU_R6], regs[CPU_R7], regs[CPU_PC]);
        log("sp={:x} pt={:x} pf={:x}", regs[CPU_SP], regs[CPU_PT], regs[CPU_PF]);
        log("ivt={:x}", regs[CPU_IVT]);
    }
    return;
}

ldr:
{
    CPU_SET_LHS_AND_GET_RHS();

    *lhs = _bus.read<uint8_t>(rhs);

    return;
}

str:
{
    CPU_GET_LHS_AND_RHS();

    _bus.write<uint8_t>(lhs, rhs);
    return;
}

add:
{
    CPU_OP(+);
    return;
}

sub:
{
    CPU_OP(-);
    return;
}

mul:
{
    CPU_OP(*);
    return;
}

div:
{
    CPU_OP(/);
    return;
}

mod:
{
    CPU_OP(%);
    return;
};

_not:
{
    CPU_SOP(~);
    return;
};

_and:
{
    CPU_OP(&);
    return;
};

_or:
{
    CPU_OP(|);
    return;
};

_xor:
{
    CPU_OP(^);
    return;
};

_push:
{
    CPU_GET_VAL();

    push(val);
    return;
};

_pop:
{
    CPU_SET_VAL();
    *val = pop();
    return;
};

jmp:
{
    CPU_GET_VAL();
    regs[CPU_PC] = val;
    return;
};

cmp:
{
    CPU_GET_LHS_AND_RHS();

    if (lhs == rhs)
    {
        flags.EQ = 1;
    }
    else
    {
        flags.EQ = 0;
    }

    if (lhs < rhs)
    {
        flags.LT = 1;
    }
    else
    {
        flags.LT = 0;
    }

    return;
};

je:
{
    CPU_GET_VAL();
    if (flags.EQ == 1)
        regs[CPU_PC] = val;

    return;
};

jne:
{
    CPU_GET_VAL();
    if (flags.EQ == 0)
        regs[CPU_PC] = val;

    return;
};

jlt:
{
    CPU_GET_VAL();

    if (flags.LT == 1)
        regs[CPU_PC] = val;
    return;
};

jgt:
{
    CPU_GET_VAL();

    if (flags.LT == 0)
        regs[CPU_PC] = val;
    return;
};

in:
{
    CPU_SET_LHS_AND_GET_RHS();

    *lhs = io::read(rhs);
    return;
};

out:
{
    CPU_GET_LHS_AND_RHS();

    io::write(lhs, rhs);
    return;
};

stw:
{
    CPU_GET_LHS_AND_RHS();

    _bus.write<uint32_t>(lhs, rhs);
    return;
};

ldw:
{
    CPU_SET_LHS_AND_GET_RHS();

    *lhs = _bus.read<uint32_t>(rhs);

    return;
};

_int:
{
    CPU_GET_VAL();

    trigger_interrupt(val, true);
    return;
}

call:
{
    CPU_GET_VAL();

    push(regs[CPU_PC]);

    regs[CPU_PC] = val;
    return;
}

mov:
{
    CPU_SET_LHS_AND_GET_RHS();

    if (lhs == &regs[CPU_IVT])
    {
        load_ivt(rhs);
    }

    else
    {
        *lhs = rhs;
    }
    return;
}
}

void Cpu::trigger_interrupt(int number, bool software)
{
    push(regs[CPU_PC]);

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

    if (ivt.entries[number].used)
    {
        regs[CPU_PC] = ivt.entries[number].address;
    }
    else
    {
        error("Unhandled interrupt: {}", number);
        regs[CPU_PC] = pop();
    }
}

void Cpu::load_ivt(uint32_t address)
{
    if (flags.PL == 0)
    {
        ivt = _ram.read<Ivt>(address);
    }
    else
    {
        trigger_interrupt(0, false);
    }
}
