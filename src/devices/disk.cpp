#include <devices/disk.hpp>
#include <fstream>
#include <log.hpp>

using namespace caar;

void DiskDevice::write(uint32_t address)
{
    auto command = _ram.read<DiskCommand>(address);

    if (command.type == 0) // Read
    {
        for (int i = 0; i < 512 * command.sectors; i++)
        {
            _ram.write<uint8_t>(command.address + i, _buf[command.offset * 512 + i]);
        }
    }

    else if (command.type == 1) // Write
    {
        error("TODO: write");
    }
}

uint32_t DiskDevice::read()
{
    return 0;
}

void DiskController::attach_disk(char *path)
{
    std::ifstream input(path, std::ios::binary);

    std::vector<uint8_t> buffer(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));

    DiskDevice *disk = new DiskDevice(_ram, buffer);

    disks.push_back(*disk);

    _bus->attach(disk);
}
