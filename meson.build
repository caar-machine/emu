project('caar-emu','cpp',
    version: '0.1',
    default_options: ['cpp_std=gnu++17', 'warning_level=2', 'werror=true', 'optimization=3'],
    license: 'MIT')

sdl2_dep = dependency('sdl2')
fmt_dep = dependency('fmt')

inc = include_directories('src')

r = run_command('git', 'rev-parse', '--short', 'HEAD', check: true)

output = r.stdout().strip()

param = '-DGIT_HASH="' + output + '"'

add_project_arguments(param, language: 'cpp')

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
    'src/devices/io.cpp',
    'src/devices/io.hpp',
    'src/devices/disk.cpp',
    'src/devices/disk.hpp',
 
    ]

install_dir = join_paths(meson.current_source_dir(), 'bin')

if meson.is_subproject()
  install_dir = join_paths(meson.current_source_dir(), '../../bin')
endif

executable('caar-emu', sources: files, include_directories: inc, install: true, install_dir: install_dir, dependencies: [sdl2_dep, fmt_dep])
