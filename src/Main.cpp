#include "Main.h"
#include "log.h"
#include <iostream>
#include "RedisCMD.h"

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

    cout << RedisCMD::ping() << endl;
    cout << RedisCMD::set("aaa", "1297ushnf") << endl;

    main.End();
    return 0;
}