#pragma once
#include <SDL2/SDL.h>
#include <cpu.hpp>
#include <devices/bus.hpp>

namespace caar
{
namespace dev
{

class Gpu : public BusDevice
{
public:
    Gpu(int width, int height);

    void write(uint32_t address);
    uint32_t read();

    void update();
    void action(uint32_t pixel, uint32_t address);

private:
    uint32_t *pixels;
    int _width = 0, _height = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};

} // namespace dev
} // namespace caar