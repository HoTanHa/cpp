#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>

char buffer[1024];
char logfilename[50] = {0};
FILE *fp;
const char *logAppName = "log";
pthread_mutex_t mutexLog;
int dbg_level = 1;

void set_log_level(int level)
{
    dbg_level = level;
}

void clear_log(void)
{
    DIR *dirLog;
    struct dirent *direntLog;
    char log1[20];
    char log2[20];
    time_t time_unix;
    struct tm tm;

    pthread_mutex_init(&mutexLog, NULL);

    mkdir(logAppName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    dirLog = opendir(logAppName);

    if (dirLog == NULL)
    {
        return;
    }
    time_unix = time(NULL);
    tm = *gmtime(&time_unix);
    sprintf(log1, "log%04d%02d%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    time_unix -= 86400;
    tm = *gmtime(&time_unix);
    sprintf(log2, "log%04d%02d%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    while ((direntLog = readdir(dirLog)) != NULL)
    {

        int len = strlen(direntLog->d_name);
        if ((len > 10) && (!strncmp(&(direntLog->d_name[len - 3]), "txt", 3)))
        {

            if ((!strcmp(direntLog->d_name, log1)) || (!strcmp(direntLog->d_name, log2)))
            {
                ;
            }
            else
            {
                sprintf(logfilename, "%s/%s", logAppName, direntLog->d_name);
                remove(logfilename);
            }
        }
    }
    closedir(dirLog);
}

void log_print(const char *filename, const char *func, int line, char *fmt, ...)
{
    va_list list;
    time_t t;
    struct tm tm;
    char *log_time;

    pthread_mutex_lock(&mutexLog);
    t = time(NULL);
    tm = *localtime(&t);

    mkdir(logAppName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    sprintf(logfilename, "%s/log%.4d%.2d%.2d.txt", logAppName, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    fp = fopen(logfilename, "a+");
    log_time = asctime(localtime(&t));
    log_time[strlen(log_time) - 1] = 0; // Getting rid of \n
    fprintf(fp, "[%s] [AppWebAPI]", log_time);

    fprintf(fp, "[%s][%s][line:%d]\t", filename, func, line);
    va_start(list, fmt);

    vsnprintf(buffer, sizeof(buffer), fmt, list);
    fprintf(fp, "%s", buffer);
    va_end(list);
    fclose(fp);

    pthread_mutex_unlock(&mutexLog);
}

void close_log(void)
{
    pthread_mutex_destroy(&mutexLog);
}
