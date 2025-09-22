#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdarg.h>
#include <stdio.h>

// Logging macros
#define logi(format, ...) _log_base(stdout, "", format, ##__VA_ARGS__)
#define logw(format, ...) _log_base(stdout, "[W]", format, ##__VA_ARGS__)
#define loge(format, ...) _log_base(stderr, "[E]", format, ##__VA_ARGS__)
#define logr(format, ...) printf(format, ##__VA_ARGS__)

extern void _log_base(FILE* stream, const char* prefix, const char* format, ...);
extern char nibble2hex(int n);
extern void to_uppercase(char* str);
extern char * to_hex_string(const char* str);

#endif // __COMMON_H__
