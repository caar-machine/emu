#include <SDL2/SDL_render.h>
#include <devices/gpu.hpp>

using namespace caar::dev;

void Gpu::write(uint32_t address)
{
    (void)address;
    log("Attempting to write to gpu at address {:x}", address);
}

inline uint32_t Gpu::read()
{
    return FB_ADDRESS;
}

void Gpu::update()
{
    SDL_UpdateTexture(texture, NULL, pixels, _width * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Gpu::action(uint32_t pixel, uint32_t address)
{
    pixels[address - FB_ADDRESS] = pixel;
}

Gpu::Gpu(int width, int height) : BusDevice(BUS_DEV_GPU), _width(width), _height(height)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("caar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    pixels = new uint32_t[FB_SIZE]();

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    SDL_RenderClear(renderer);
}
