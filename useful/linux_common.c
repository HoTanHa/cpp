#include "linux_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Get the tick count in milliseconds since some unspecified starting point (often the system boot time).
 *
 * @return long The tick count in milliseconds.
 */
long get_tick_count_milisec()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((ts.tv_nsec / 1000000) + (ts.tv_sec * 1000l));
}

/**
 * @brief Get the tick count sec object
 *
 * @return long The tick count in seconds.
 */
long get_tick_count_sec()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec);
}

/**
 * @brief get the unix timestamp from a string in the format "YYYYMMDDhhmmss"
 *
 * @param sYYYYMMDDhhmmss
 * @return long
 */
long str_to_timeUnix(char *sYYYYMMDDhhmmss)
{
    if (strlen(sYYYYMMDDhhmmss) < 14)
    {
        return 0;
    }
    for (size_t i = 0; i < 14; i++)
    {
        if (!isdigit(sYYYYMMDDhhmmss[i]))
        {
            return 0;
        }
    }

    time_t rawtime;
    struct tm timeinfo;
    int year, month, day, hour, minute, second;
    int len = 0;
    char *sTime = sYYYYMMDDhhmmss;
    year = (sTime[len] - 0x30) * 1000 + (sTime[len + 1] - 0x30) * 100 + (sTime[len + 2] - 0x30) * 10 + (sTime[len + 3] - 0x30);
    len += 4;
    month = (sTime[len] - 0x30) * 10 + (sTime[len + 1] - 0x30);
    len += 2;
    day = (sTime[len] - 0x30) * 10 + (sTime[len + 1] - 0x30);
    len += 2;
    hour = (sTime[len] - 0x30) * 10 + (sTime[len + 1] - 0x30);
    len += 2;
    minute = (sTime[len] - 0x30) * 10 + (sTime[len + 1] - 0x30);
    len += 2;
    second = (sTime[len] - 0x30) * 10 + (sTime[len + 1] - 0x30);
    time(&rawtime);
    gmtime_r(&rawtime, &timeinfo);
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = minute;
    timeinfo.tm_sec = second;
    timeinfo.tm_zone = 0;
    rawtime = mktime(&timeinfo);
    return rawtime;
}

/**
 * @brief Get the string hex object
 *
 * @param buff
 * @param length
 * @return char* The hexadecimal string representation of the buffer.
 *         AA.BB.CC. format. Free the returned string after use.
 */

static unsigned char hex_digits[] = "0123456789ABCDEF";
char *get_string_hex(uint8_t *buff, int length)
{
    int lenHex = length * 3 + 2;
    char *strHex = (char *)malloc(lenHex);
    char *strTmp = strHex;
    memset(strHex, 0, lenHex);
    for (int i = 0; i < length; i++)
    {
        strTmp[0] = hex_digits[(buff[i] >> 4) & 0x0F];
        strTmp[1] = hex_digits[buff[i] & 0x0F];
        strTmp[2] = '.';
        strTmp += 3;
    }
    strHex[lenHex - 1] = '\0';
    return strHex;
}

/**
 * @brief count the number of bits set to 1 in a byte
 *
 * @param byte
 * @return uint8_t
 */
uint8_t count_bit_set(uint8_t byte)
{
    static const uint8_t NIBBLE_LOOKUP[16] =
        {
            0, 1, 1, 2, 1, 2, 2, 3,
            1, 2, 2, 3, 2, 3, 3, 4};

    return NIBBLE_LOOKUP[byte & 0x0F] + NIBBLE_LOOKUP[byte >> 4];
}

static const unsigned char BitsSetTable256[256] =
    {
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
        B6(0), B6(1), B6(1), B6(2)};

/**
 * @brief count the number of bits set to 1 in a 32-bit unsigned integer
 *
 * @param numU32 The 32-bit unsigned integer to count bits in.s
 * @return uint32_t
 */
uint32_t count_bit_set_uint32(uint32_t numU32)
{
    uint32_t c;
    c = BitsSetTable256[numU32 & 0xff] +
        BitsSetTable256[(numU32 >> 8) & 0xff] +
        BitsSetTable256[(numU32 >> 16) & 0xff] +
        BitsSetTable256[numU32 >> 24];
    // unsigned char *p = (unsigned char *)&numU32;
    // c = BitsSetTable256[p[0]] +
    //     BitsSetTable256[p[1]] +
    //     BitsSetTable256[p[2]] +
    //     BitsSetTable256[p[3]];
    return c;
}

/**
 * @brief Get the filesize object
 *
 * @param filename
 * @return long
 */
long get_filesize(const char *filename)
{
    if (filename == NULL)
    {
        return 0;
    }

    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        return 0;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

#include <sys/stat.h>

/**
 * @brief Check if the given path is a regular file.
 *
 * @param path The path to check.
 * @return true if the path is a regular file, false otherwise.
 */
int is_regular_file(const char *path)
{
    if (access(path, F_OK) == 0)
    {
        struct stat path_stat;
        if (stat(path, &path_stat) == 0)
        {
            return S_ISREG(path_stat.st_mode);
        }
    }
    return 0;
}

/**
 * @brief Check if the given file exists.
 *
 * @param path The path to check.
 * @return true if the file exists, false otherwise.
 */
int is_file_exist(const char *path)
{
    return (access(path, F_OK) == 0);
}

#include <openssl/md5.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char *get_string_md5sum(unsigned char *md)
{
    char *sTmp = (char *)malloc(MD5_DIGEST_LENGTH * 2 + 1);
    memset(sTmp, 0, (MD5_DIGEST_LENGTH * 2 + 1));
    int i;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf(sTmp + 2 * i, "%02x", md[i]);
    }
    return (sTmp);
}

/**
 * @brief calculate md5 sum of a file
 *
 * @param path : path of the file
 * @return char* : result md5. free after use if it is not NULL
 */
char *calculate_md5_file_c(const char *path)
{
    int fd;
    unsigned long file_size;
    char *file_buffer;
    unsigned char *result = NULL;

    fd = open(path, O_RDONLY);
    if (fd <= 0)
    {
        return NULL;
    }

    file_size = get_filesize(path);
    if (file_size <= 0)
    {
        return NULL;
    }

    result = (uint8_t *)malloc(MD5_DIGEST_LENGTH);
    file_buffer = (char *)mmap(0, file_size, PROT_READ, MAP_SHARED, fd, 0);
    MD5((unsigned char *)file_buffer, file_size, result);
    munmap(file_buffer, file_size);

    char *md5sum = get_string_md5sum(result);
    free(result);
    return md5sum;
}
