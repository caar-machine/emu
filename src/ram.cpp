#include <ram.hpp>

using namespace caar;

template <>
void Ram::write<uint8_t>(uint32_t address, uint8_t val)
{
    buffer[address] = val;
}

template <>
void Ram::write<uint16_t>(uint32_t address, uint16_t val)
{
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&val);

    buffer[address] = bytes[0];
    buffer[address + 1] = bytes[1];
}

template <>
void Ram::write<uint32_t>(uint32_t address, uint32_t val)
{
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&val);

    buffer[address] = bytes[0];
    buffer[address + 1] = bytes[1];
    buffer[address + 2] = bytes[2];
    buffer[address + 3] = bytes[3];
}

void Ram::write_chunk(std::vector<char> buffer, uint32_t address)
{
    for (std::size_t i = 0; i < buffer.size(); i++)
    {
        write<uint8_t>(address + i, buffer[i]);
    }
}