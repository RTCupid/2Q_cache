#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
    printf(GRN "[DEBUG] %s:%d: " fmt RESET, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

#endif
