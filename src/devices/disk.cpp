#include <devices/disk.hpp>
#include <fstream>
#include <log.hpp>

using namespace caar;

void DiskDevice::write(uint32_t address)
{
    uint8_t bytes[6] = {0};

    bytes[0] = _ram.read<uint8_t>(address);
    bytes[1] = _ram.read<uint8_t>(address + 1);
    bytes[2] = _ram.read<uint8_t>(address + 2);
    bytes[3] = _ram.read<uint8_t>(address + 3);
    bytes[4] = _ram.read<uint8_t>(address + 4);
    bytes[5] = _ram.read<uint8_t>(address + 5);

    auto command = reinterpret_cast<DiskCommand *>(bytes);

    if (command->type == 0) // Read
    {
        for (int i = 0; i < 512 * command->sectors; i++)
        {
            _ram.write<uint8_t>(command->address + i, _buf[i]);
        }
    }

    else if (command->type == 1) // Write
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
