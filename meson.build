project('caar-emu','cpp',
    version: '0.1',
    default_options: ['cpp_std=gnu++17', 'warning_level=3', 'werror=true'],
    license: 'MIT')

sdl2_dep = dependency('sdl2')
fmt_dep = dependency('fmt')

inc = include_directories('src')

add_global_arguments('-DGITHASH=$(git rev-parse --short HEAD)', language : 'cpp')

files = [
    'src/cpu.cpp',
    'src/cpu.hpp',
    'src/log.hpp',
    'src/opts.cpp',
    'src/opts.hpp',
    'src/ram.cpp',
    'src/ram.hpp',
    'src/main.cpp',
    'src/devices/gpu.hpp',
    'src/devices/gpu.cpp',
    'src/devices/bus.hpp',
    'src/devices/bus.cpp',
    ]

executable('caar-emu', sources: files, include_directories: inc, dependencies: [sdl2_dep, fmt_dep])