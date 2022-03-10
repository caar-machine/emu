#pragma once
#include <devices/bus.hpp>

namespace caar
{

struct [[gnu::packed]] DiskCommand
{
    uint8_t type;
    uint8_t sectors;
    uint32_t offset;
    uint32_t address;
};

class DiskDevice : public BusDevice
{
public:
    DiskDevice(Ram &ram, std::vector<uint8_t> buf) : BusDevice(BUS_DEV_DISK), _ram(ram), _buf(buf){};

    void write(uint32_t address) override;

    uint32_t read(void) override;

    Bus *_bus;

    DeviceType type = BUS_DEV_DISK;

private:
    Ram &_ram;
    std::vector<uint8_t> _buf;
};

class DiskController
{
public:
    DiskController(Ram &ram, Bus *bus) : _bus(bus), _ram(ram) {}

    void attach_disk(char *name);

private:
    Bus *_bus;
    Ram &_ram;
    std::vector<DiskDevice> disks;
};

} // namespace caar
