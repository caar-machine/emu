#pragma once
#include <cstdint>
#include <opts.hpp>
#include <vector>

#define MEMORY_SIZE (uint32_t)(program_options::mem_size() * 1024 * 1024)
#define FB_WIDTH 1024
#define FB_HEIGHT 768
#define FB_SIZE FB_WIDTH *FB_HEIGHT

#define STACK_SIZE 8192

#define FB_ADDRESS (uint32_t)(MEMORY_SIZE - STACK_SIZE - FB_SIZE)

namespace caar
{
class Ram
{
public:
    Ram(uint32_t size) : _size(size)
    {
        buffer = (uint8_t *)calloc(size, 1);
    }

    template <typename T>
    T read(uint32_t address)
    {
        return *reinterpret_cast<T *>(buffer + address);
    }

    template <typename T>
    void write(uint32_t address, T val);

    void write_chunk(std::vector<char> buffer, uint32_t address);

private:
    uint8_t *buffer;
    uint32_t _size;
};

} // namespace caar
