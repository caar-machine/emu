#pragma once
#include <devices/bus.hpp>
#include <ram.hpp>

namespace caar
{

class Cpu
{
public:
    Cpu(Ram &ram, Bus &bus) : _ram(ram), _bus(bus){};
    void do_cycle();
    void trigger_interrupt(int number, bool software = true);

private:
    Ram &_ram;
    Bus &_bus;
};

} // namespace caar