#ifndef __LOG__
#define __LOG__

#include <stdio.h>
#include <string.h>
#include <time.h>

// e.g. LOG_DEBUG("main", "test %s %d %f", "hello", 999, 0.123);

#define LOG_DEBUG(...)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        char sBuff[32] = {0};                                                                                          \
        time_t t = time(NULL);                                                                                         \
        strftime(sBuff, sizeof(sBuff), "%y-%m-%d %H:%M:%S", localtime(&t));                                            \
        char sOut[512] = {0};                                                                                          \
        sprintf(sOut, "%s [DEBUG] [%s:%d] ", sBuff, __FILE__, __LINE__);                                               \
        printf(sOut);                                                                                                  \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n");                                                                                                  \
    } while (0)

#define LOG_ERROR(...)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        char sBuff[32] = {0};                                                                                          \
        time_t t = time(NULL);                                                                                         \
        strftime(sBuff, sizeof(sBuff), "%y-%m-%d %H:%M:%S", localtime(&t));                                            \
        char sOut[512] = {0};                                                                                          \
        sprintf(sOut, "%s [ERROR] [%s:%d] ", sBuff, __FILE__, __LINE__);                                               \
        printf(sOut);                                                                                                  \
        printf(__VA_ARGS__);                                                                                           \
        printf("\n");                                                                                                  \
    } while (0)

#endif //!__LOG__
