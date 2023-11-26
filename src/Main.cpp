#include "Main.h"
#include "Task.h"
#include "log.h"
#include "RedisCMD.h"
#include <iostream>

using namespace std;

bool Main::Start()
{
    const char *hostname = "127.0.0.1";
    unsigned short port = 6379;

    if (RedisCMD::Connect(hostname, port) == false)
    {
        LOG_ERROR("Redis connect error");
        return false;
    }

    return true;
}

void Main::End()
{
    RedisCMD::DisConnect();
}



///////////////////////////////////////////////////////////////

#define TEST
#ifdef TEST
#include "test/test.h"
#endif

int main()
{
    LOG_ERROR("hello %s %f", "world", 999.666);

    Main main;

    if (main.Start() == false)
    {
        LOG_ERROR("Main init error");
        main.End();
        return 0;
    }

    // cout << RedisCMD::ping() << endl;
    // cout << RedisCMD::set("aaa", "1297ushnf") << endl;

    int iU1 = 10002;

    Task task;
    task.RecvLoginReq(iU1);
    task.RecvDisconnectReq(iU1);


    main.End();
    return 0;
}