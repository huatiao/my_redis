///////////////////////////////////////////////////////////////

#include "base_head.h"
#include "Main.h"


#define TEST
#ifdef TEST
#include "test/test.h"
#include "RedisCMD.h"
#include "Task.h"
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
    // RedisCMD::set("ccc", "agfiunf");
    // RedisCMD::hset("test:1001", "field2", "vvvv");
    string aa = RedisCMD::hget("test:1001", "field1");
    cout << aa << endl;

    // int iU1 = 10002;

    // Task task;
    // task.RecvLoginReq(iU1);
    // task.RecvDisconnectReq(iU1);

    main.End();
    return 0;
}