#pragma once
#include <cstdint>

namespace caar
{
namespace io
{
uint32_t read(uint8_t port);
void write(uint8_t port, uint8_t data);
} // namespace io
} // namespace caar
