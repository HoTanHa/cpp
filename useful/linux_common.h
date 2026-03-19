#ifndef __LINUX_COMMON_FUNCTION_H__
#define __LINUX_COMMON_FUNCTION_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

// tick, time
long get_tick_count_milisec();
long get_tick_count_sec();
long str_to_timeUnix(char *sYYYYMMDDhhmmss);

// hex string
char *get_string_hex(uint8_t *buff, int length);

// bit
uint8_t count_bit_set(uint8_t byte);
uint32_t count_bit_set_uint32(uint32_t numU32);

// file
long get_filesize(const char *filename);
int is_regular_file(const char *path);
int is_file_exist(const char *path);
char *calculate_md5_file_c(const char *path);

#ifdef __cplusplus
}
#endif
#endif //__LINUX_COMMON_FUNCTION_H__