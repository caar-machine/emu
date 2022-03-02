#pragma once

#include <algorithm>
#include <array>
#include <log.hpp>
#include <ram.hpp>
#include <stdexcept>

namespace caar
{

#define ALIGN_DOWN(NUM, WHAT) ((NUM) & ~(WHAT - 1))

class Bus;

enum DeviceType
{
    BUS_DEV_EMPTY,
    BUS_DEV_DISK,
    BUS_DEV_GPU,
};

struct [[gnu::packed]] RawBusDevice
{
    uint32_t type;
    uint32_t addr;
};

struct [[gnu::packed]] RawBus
{
    uint32_t device_count = 0;
    RawBusDevice devices[32] = {};
};

struct BusDevice
{
    BusDevice(DeviceType _type) : type(_type){};

    virtual void write(uint32_t address) { (void)address; }
    virtual uint32_t read() { return 0; }
    virtual ~BusDevice() = 0;

    virtual void action(uint32_t param, uint32_t param1)
    {
        (void)param;
        (void)param1;
    }

    Bus *_bus;
    DeviceType type = BUS_DEV_EMPTY;
}; // namespace caar

class Bus
{
public:
    Bus(Ram &ram) : _ram(ram)
    {
        raw_bus = new RawBus;
    }

    void attach(BusDevice *dev);

    template <typename T>
    void write(uint32_t address, T val)
    {

        if (address >= MEMORY_SIZE + 1 && address < MEMORY_SIZE + 0x1000)
        {
            error("attempt to write at bus");
        }

        else if (address >= FB_ADDRESS && address <= FB_ADDRESS + FB_SIZE)
        {
            BusDevice **device = std::find_if(devices, devices + 32, [](BusDevice *dev) -> bool
                                              {
                                                  if (dev->type == BUS_DEV_GPU)
                                                  {
                                                      return true;
                                                  }

                                                  return false;
                                              });
            (*device)->action(val, address);
        }

        else if (address >= MEMORY_SIZE + 0x1000 && address < MEMORY_SIZE + 0x2000)
        {
            size_t index = address - MEMORY_SIZE - 0x1000;

            if (index > device_num)
                error("Out of bounds write to bus.");

            devices[index]->write(val);
        }

        else
        {
            _ram.write<T>(address, val);
        }
    }

    template <typename T>
    T read(uint32_t address)
    {
        if (address >= MEMORY_SIZE + 1 && address < MEMORY_SIZE + 0x1000)
        {
            RawBus *raw = get_raw_bus();

            return *reinterpret_cast<T *>((uintptr_t)raw + (address - (MEMORY_SIZE + 1)));
        }

        else if (address >= MEMORY_SIZE + 0x1000 && address < MEMORY_SIZE + 0x2000)
        {

            size_t index = address - MEMORY_SIZE - 0x1000;

            if (index <= 128)
            {
                index = 0;
            }

            else
            {
                index = ALIGN_DOWN(index, 128) / 128;
            }

            if (index > device_num)
            {
                error("Out of bounds read to bus.");
            }

            return devices[index]->read();
        }

        else
        {
            return _ram.read<T>(address);
        }

        throw std::runtime_error("Couldn't read from address");
    }

    RawBus *get_raw_bus();

private:
    size_t device_num = 0;
    BusDevice *devices[32];
    RawBus *raw_bus;

private:
    Ram &_ram;
};
} // namespace caar
