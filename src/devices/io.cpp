#include <cstdio>
#include <devices/io.hpp>
#include <log.hpp>
#include <ram.hpp>

uint32_t caar::io::read(uint8_t port)
{
    switch (port)
    {
    case 0:
        return getchar();
    case 1:
        return MEMORY_SIZE + 1;

    case 2: // Cpu ID, 1 is caar1
        return 1;
    case 3: // running in VM
        return 1;

    default:
        break;
    }
    return 0;
}

void caar::io::write(uint8_t port, uint8_t data)
{
    switch (port)
    {
    case 0:
        putchar(data);
        break;
    case 1:
        log("Cannot write to bus");
        break;
    }
}
