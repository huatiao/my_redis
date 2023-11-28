#include "RedisCMD.h"
#include "hiredis/hiredis.h"
#include "hiredis/read.h"
#include "log.h"
#include <cstring>
#include <iostream>

redisContext *RedisCMD::s_pContext = nullptr;

bool RedisCMD::Connect(const char *hostname, unsigned short port)
{
    struct timeval timeout = {1, 500000}; // 1.5 seconds
    s_pContext = redisConnectWithTimeout(hostname, port, timeout);
    if (s_pContext == NULL || s_pContext->err)
    {
        if (s_pContext)
        {
            std::cerr << "Connection error: " << s_pContext->errstr << std::endl;
            redisFree(s_pContext);
        }
        else
        {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
    }
    else
    {
        std::cout << "Redis is Connected!\n" << std::endl;
    }

    // authenticate passward
    redisReply *reply = (redisReply *)redisCommand(s_pContext, "AUTH dhi12Hg"); // AUTH passward
    if (reply == NULL || reply->type == REDIS_REPLY_ERROR)
    {
        LOG_ERROR("Authentication failed: %s", reply ? reply->str : "NULL");
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);

    return true;
}

void RedisCMD::DisConnect()
{
    if (s_pContext)
    {
        redisFree(s_pContext);
    }

    s_pContext = nullptr;
}

string RedisCMD::ping()
{
    if (s_pContext == nullptr)
    {
        LOG_ERROR("RedisCMD::ping s_pContext is nullptr");
        return "";
    }

    redisReply *reply = (redisReply *)redisCommand(s_pContext, "PING");
    string result = reply->str;
    freeReplyObject(reply);
    return result;
}

bool RedisCMD::set(const char *sKey, const char *sValue)
{
    if (s_pContext == nullptr)
    {
        LOG_ERROR("RedisCMD::set s_pContext is nullptr");
        return false;
    }
    redisReply *pReply = (redisReply *)redisCommand(s_pContext, "SET %s %s", sKey, sValue);
    bool bRet = false;
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_STATUS)
        {
            if (strcmp(pReply->str, "OK") == 0)
            {
                bRet = true;
            }
        }
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            LOG_ERROR("RedisCMD::set error pReply->str = %s", pReply->str);
        }
    }

    if (bRet == false)
    {
        LOG_ERROR("RedisCMD::set failure");
    }

    freeReplyObject(pReply);
    return bRet;
}