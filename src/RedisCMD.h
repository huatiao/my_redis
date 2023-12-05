#ifndef __REDIS_REDISCMD_H__
#define __REDIS_REDISCMD_H__

#include "hiredis/hiredis.h"
#include <string>

using std::string;

class RedisCMD {
public:
    static redisContext *s_pContext;

    static bool Connect(const char *hostname, unsigned short port, const char *password = nullptr);
    static void DisConnect();

    static bool ErrorCheck(const char *pKey);

    // 命令比较特殊，统一用小写
    static bool set(const char *pKey, const char *pValue);
    static string get(const char *pKey);
    
    //one time, one field
    static bool hset(const char *pKey, const char *pfield, const char *pValue);
    static string hget(const char *pKey, const char *pfield);

    // Add other commands as needed
};

#endif
