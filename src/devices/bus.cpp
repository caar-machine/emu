#include <devices/bus.hpp>

using namespace caar;

void Bus::attach(BusDevice *dev)
{
    devices[device_num++] = dev;

    if (dev->type == BUS_DEV_GPU)
    {
        gpu = dev;
    }
}

BusDevice::~BusDevice() {}

RawBus *Bus::get_raw_bus()
{
    if (raw_bus->device_count != device_num)
    {
        raw_bus->device_count = device_num;

        for (size_t i = 0; i < device_num; i++)
        {
            raw_bus->devices[i].type = devices[i]->type;

            raw_bus->devices[i].addr = i + MEMORY_SIZE + 0x1000;
        }
    }

    return raw_bus;
}
