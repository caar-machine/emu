#pragma once
#include <cstring>
#include <fmt/color.h>
#include <fmt/format.h>

enum LogLevel
{
    LOG_INFO,
    LOG_ERROR,
};

inline void vlog(LogLevel level, const char *file, int line, fmt::string_view format, fmt::format_args args)
{
    fmt::print(fg(level == LOG_INFO ? fmt::color::green : fmt::color::red), level == LOG_INFO ? "info" : "error");
    fmt::print(fg(fmt::color::gray), " {}:{} ", basename(file), line);
    fmt::vprint(format, args);
    fmt::print("\n");
}

template <typename S, typename... Args>
inline void log_impl(LogLevel level, const char *file, int line, const S &format, Args &&...args)
{
    vlog(level, file, line, format, fmt::make_format_args(args...));
}

#define log(fmt, ...) \
    log_impl(LOG_INFO, __FILE__, __LINE__, FMT_STRING(fmt) __VA_OPT__(, ) __VA_ARGS__)

#define error(fmt, ...) \
    log_impl(LOG_ERROR, __FILE__, __LINE__, FMT_STRING(fmt) __VA_OPT__(, ) __VA_ARGS__)
