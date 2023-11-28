#ifndef __REDIS_REDISCMD_H__
#define __REDIS_REDISCMD_H__

#include "hiredis/hiredis.h"
#include <string>

using std::string;

class RedisCMD {
public:
    static redisContext *s_pContext;

    static bool Connect(const char *hostname, unsigned short port);
    static void DisConnect();

    // 命令比较特殊，统一用小写
    static string ping();
    static bool set(const char *sKey, const char *sValue);
    

    // static string GET() {
    //     redisReply *reply = (redisReply*)redisCommand(s_pContext,"GET foo");
    //     string result = reply->str;
    //     freeReplyObject(reply);
    //     return result;
    // }
    // Add other commands as needed
};

#endif
