#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

/*
// extern int dbg_level;
// void set_log_level(int level);
// void clear_log(void);
// void close_log(void);
// void log_print(const char *filename, const char *func, int line, ...);

// #define info_msg(fmt, ...)                                                                          \
//     do                                                                                              \
//     {                                                                                               \
//         if (dbg_level == 1)                                                                         \
//             printf("[INFO]\t[%s]_[%s]_%d:\t" fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
//         else if (dbg_level > 1)                                                                     \
//             log_print(__FILE__, __FUNCTION__, __LINE__, "[IF] " fmt, ##__VA_ARGS__);                \
//     } while (0)

// #define error_msg(fmt, ...)                                                          \
//     do                                                                               \
//     {                                                                                \
//         if (dbg_level == 1)                                                          \
//             printf("[ERROR]\t[%s]_%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);     \
//         else if (dbg_level > 1)                                                      \
//             log_print(__FILE__, __FUNCTION__, __LINE__, "[ER] " fmt, ##__VA_ARGS__); \
//     } while (0)

// #define debug_msg(fmt, ...)                                                          \
//     do                                                                               \
//     {                                                                                \
//         if (dbg_level == 1)                                                          \
//             printf("[DEBUG]\t[%s]_%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);     \
//         else if (dbg_level > 1)                                                      \
//             log_print(__FILE__, __FUNCTION__, __LINE__, "[DB] " fmt, ##__VA_ARGS__); \
//     } while (0)
*/
#define info_msg(fmt, ...)                                                                      \
    do                                                                                          \
    {                                                                                           \
        printf("[INFO]\t[%s]_[%s]_%d:\t" fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define error_msg(fmt, ...)                                                  \
    do                                                                       \
    {                                                                        \
        printf("[ERROR]\t[%s]_%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define debug_msg(fmt, ...)                                                  \
    do                                                                       \
    {                                                                        \
        printf("[DEBUG]\t[%s]_%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#endif // __LOG_H__