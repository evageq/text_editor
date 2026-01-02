#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "debug.h"

static FILE* g_log_fd;

int
debug_init()
{
    g_log_fd = fopen(LOG_FILE, "w");
    if (g_log_fd == NULL)
    {
        fprintf(stderr, "Failed to init debugger\n");
        return -1;
    }

    return 0;
}

void
debug(const char *fmt, ...)
{
    enum {
        TIME_STR_LEN = 64,
        DEBUG_MSG_LEN = 1024
    };

    FILE* log_fd = (g_log_fd == NULL ? stderr : g_log_fd);
    time_t raw_time;
    struct tm *tm;
    char time_str[TIME_STR_LEN] = {0};

    char debug_msg[DEBUG_MSG_LEN];
    char debug_msg_fmt[DEBUG_MSG_LEN];

    va_list args;

    raw_time = time(NULL); 
    tm = localtime(&raw_time);
    strftime(time_str, TIME_STR_LEN, "%H:%M:%S", tm);

    va_start(args, fmt);

    snprintf(debug_msg, DEBUG_MSG_LEN, "[DEBUG][%s]: %s\n", time_str, fmt);
    vfprintf(log_fd, debug_msg, args);
    fflush(log_fd);

    va_end(args);
}

void
debug_close()
{
    if (g_log_fd != NULL)
    {
        fclose(g_log_fd);
    }
}
