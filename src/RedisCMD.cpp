#include "RedisCMD.h"
#include "hiredis/hiredis.h"
#include "hiredis/read.h"
#include "log.h"
#include <cstring>
#include <iostream>

#define ERROR_CHECK_WRITE(cmd)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (s_pContext == nullptr)                                                                                     \
        {                                                                                                              \
            LOG_ERROR("RedisCMD::%s s_pContext is nullptr", cmd);                                                      \
            return false;                                                                                              \
        }                                                                                                              \
    } while (0)

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

bool RedisCMD::set(const char *pKey, const char *pValue)
{
    ERROR_CHECK_WRITE("SET");

    redisReply *pReply = (redisReply *)redisCommand(s_pContext, "SET %s %s", pKey, pValue);
    bool ret = false;
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_STATUS)
        {
            if (strcmp(pReply->str, "OK") == 0)
            {
                ret = true;
                LOG_DEBUG("RedisCMD::set success key = %s", pKey);
            }
        }
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            LOG_ERROR("RedisCMD::set error pReply->str = %s", pReply->str);
        }
    }

    if (ret == false)
        LOG_ERROR("RedisCMD::set failure key = %s", pKey);

    freeReplyObject(pReply);
    return ret;
}


bool RedisCMD::hset(const char *pKey, const char *pfield, const char *pValue)
{
    ERROR_CHECK_WRITE("HSET");

    redisReply *pReply = (redisReply *)redisCommand(s_pContext, "HSET %s %s %s", pKey, pfield, pValue);
    bool ret = false;
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_INTEGER)
        {
            ret = true;
            if (pReply->integer == 1)
                LOG_DEBUG("RedisCMD::hset success:create key=%s,field=%s", pKey, pfield);
            else if (pReply->integer == 0)
                LOG_DEBUG("RedisCMD::hset success:modify key=%s,field=%s", pKey, pfield);
        }
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            LOG_ERROR("RedisCMD::hset error pReply->str = %s", pReply->str);
        }
    }

    if (ret == false)
        LOG_ERROR("RedisCMD::hset failure key = %s", pKey);

    freeReplyObject(pReply);
    return ret;
}