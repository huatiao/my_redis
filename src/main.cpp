#include "hiredis/hiredis.h"
#include <stdio.h>
// #include "RedisCommand.h"
#include "log.h"

int main()
{
    redisContext *rc = redisConnect("127.0.0.1", 6379);
    if (rc == NULL || rc->err)
    {
        printf("connection error\n");
        if (rc->err)
        {
            redisFree(rc);
        }
        return 0;
    }
    else
    {
        printf("Sucess\n");
        redisFree(rc);
    }

    LOG_ERROR("hello %s %f", "world", 999.666);

    // RedisCommand cmd("localhost", 6379);
    // std::cout << "PING: " << cmd.Ping() << std::endl;
    // std::cout << "SET: "  << cmd.SET() << std::endl;
    // std::cout << "GET: "  << cmd.GET() << std::endl;
    return 0;
}
