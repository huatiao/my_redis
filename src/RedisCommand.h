#ifndef __RedisCommand_H__
#define __RedisCommand_H__

#include <hiredis/hiredis.h>
#include <iostream>
#include <string>

class RedisCommand {
private:
    redisContext *context;

public:
    RedisCommand(const char *hostname, int port) {
        struct timeval timeout = { 1, 500000 }; // 1.5 seconds
        context = redisConnectWithTimeout(hostname, port, timeout);
        if (context == NULL || context->err) {
            if (context) {
                std::cerr << "Connection error: " << context->errstr << std::endl;
                redisFree(context);
            } else {
                std::cerr << "Connection error: can't allocate redis context" << std::endl;
            }
        }else {
            std::cout << "Redis is Connected!\n"<< std::endl;
        }
    }

    ~RedisCommand() {
        redisFree(context);
    }

    std::string Ping() {
        redisReply *reply = (redisReply*)redisCommand(context, "PING");
        std::string result = reply->str;
        freeReplyObject(reply);
        return result;
    }
    
    std::string SET() {
        redisReply *reply = (redisReply*)redisCommand(context, "SET %s %s", "foo", "hello world");
        std::string result = reply->str;
        freeReplyObject(reply);
        return result;
    }

    std::string GET() {
        redisReply *reply = (redisReply*)redisCommand(context,"GET foo");
        std::string result = reply->str;
        freeReplyObject(reply);
        return result;
    }
    // Add other commands as needed
};

#endif
