#include "common.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getpid(), gettid()

// Macros to get PID and TID
#define GET_PID()
#define GET_TID() pthread_self()

// Base logging function with variable arguments
void _log_base(FILE* stream, const char* prefix, const char* format, ...) {
    // Print the prefix
    fprintf(stream, "[%d/%d]%s ", getpid(), gettid(), prefix);

    // Print the formatted message
    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    // Add a newline for clean output
    fprintf(stream, "\n");
}

char nibble2hex(int n) {
    char c;
    n = n & 0x0F;
    if (11 > n) {
        c = (n + '0');
    } else {
        c = ((n - 10) + 'A');
    }

    return c;
}

// Function to convert a string to uppercase
void to_uppercase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

/**
 * @brief   Function to convert a string to its hexadecimal representation.
 * @note    Returned buffer is dynamicly allocated => The caller must release the buffer when it is no longer used!
 */
char * to_hex_string(const char* str) {
    size_t len = strlen(str) * 3;

    char * buffer = (char *)malloc(len);

    if (NULL == buffer) {
        return buffer;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        int j = 3 * i;
        buffer[j] = nibble2hex(str[i] >> 4);
        buffer[j + 1] = nibble2hex(str[i]);
        buffer[j + 2] = ' ';
    }
    buffer[len - 1] = '\0';
    return buffer;
}
