#include "Main.h"
#include "RedisCMD.h"
#include "base_head.h"
#include "log.h"
#include <cstdio>
#include <fstream>
#include <unistd.h>
using namespace std;

#define CONF_PATH "../etc/"

bool Main::Start()
{
    char fileName[128] = {0};
    sprintf(fileName, "%sredis.json", CONF_PATH);
    std::ifstream ifs (fileName);
    if (ifs.is_open() == false)
    {
        LOG_ERROR("open %s error", fileName);
        return false;
    }

    json redisConf = json::parse(ifs);
    ifs.close();
    string hostname = redisConf.value("hostname", "");  //Using .value replace .at or [], will not throw exceptions
    int port = redisConf.value("port", 0);
    string password = redisConf.value("password", "");

    if (RedisCMD::Connect(hostname.c_str(), port, password.c_str()) == false)
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

