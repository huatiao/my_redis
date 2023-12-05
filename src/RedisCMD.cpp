#include "RedisCMD.h"
#include "hiredis/hiredis.h"
#include "hiredis/read.h"
#include "log.h"
#include <cstring>
#include <iostream>

redisContext *RedisCMD::s_pContext = nullptr;

bool RedisCMD::ErrorCheck(const char *pCMD)
{
    if (s_pContext == nullptr)
    {
        LOG_ERROR("RedisCMD::%s s_pContext is nullptr", pCMD);
        return false;
    }
}

bool RedisCMD::Connect(const char *hostname, unsigned short port, const char *password)
{
    struct timeval timeout = {1, 500000}; // 1.5 seconds
    s_pContext = redisConnectWithTimeout(hostname, port, timeout);
    bool bSuccess = true;
    if (s_pContext)
    {
        if (s_pContext->err != 0)
        {
            bSuccess = false;
            LOG_ERROR("RedisCMD::Connect ERROR: %s", s_pContext->errstr);
        }
        else
        {
            printf("Redis is Connected!\n");
        }
    }
    else
    {
        LOG_ERROR("RedisCMD::Connect ERROR is null");
    }
    
    if (strlen(password) > 0)
    {
        // authenticate passward
        redisReply *pReply = (redisReply *)redisCommand(s_pContext, "AUTH %s", password);
        if (pReply == NULL || pReply->type == REDIS_REPLY_ERROR)
        {
            bSuccess = false;
            LOG_ERROR("RedisCMD::Connect Authentication failed: %s", pReply ? pReply->str : "NULL");
        }
        freeReplyObject(pReply);
    }
    
    if (bSuccess == false)
    {
        redisFree(s_pContext);
        s_pContext = nullptr;
    }
    return bSuccess;
}

void RedisCMD::DisConnect()
{
    if (s_pContext)
    {
        redisFree(s_pContext);
    }

    s_pContext = nullptr;
}

bool RedisCMD::set(const char *pKey, const char *pValue)
{
    if (RedisCMD::ErrorCheck("SET") == false)
        return false;

    redisReply *pReply = (redisReply *)redisCommand(s_pContext, "SET %s %s", pKey, pValue);
    bool ret = false;
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_STATUS)
        {
            if (strcmp(pReply->str, "OK") == 0)
            {
                ret = true;
                LOG_DEBUG("RedisCMD::SET success key=%s", pKey);
            }
        }
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            LOG_ERROR("RedisCMD::SET error pReply->str=%s", pReply->str);
        }
    }

    if (ret == false)
        LOG_ERROR("RedisCMD::SET failure key = %s", pKey);

    freeReplyObject(pReply);
    return ret;
}

string RedisCMD::get(const char *pKey)
{
    if (RedisCMD::ErrorCheck("GET") == false)
        return "";

    redisReply *pReply = (redisReply *)redisCommand(s_pContext, "GET %s", pKey);
    string ret;
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_STRING)
        {
            ret = pReply->str;
            LOG_DEBUG("RedisCMD::GET success key=%s val=%s", pKey, ret.c_str());
        }
        else if (pReply->type == REDIS_REPLY_NIL)
        {
            LOG_ERROR("RedisCMD::GET key[%s] value is null", pKey);
        }
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            LOG_ERROR("RedisCMD::GET error key[%s] pReply->str[%s]", pKey, pReply->str);
        }

        freeReplyObject(pReply);
    }
    else 
    {
        LOG_ERROR("RedisCMD::GET failure key=%s", pKey);
    }

    return std::move(ret);
}

bool RedisCMD::hset(const char *pKey, const char *pfield, const char *pValue)
{
    if (RedisCMD::ErrorCheck("HSET") == false)
        return false;

    redisReply *pReply = (redisReply *)redisCommand(s_pContext, "HSET %s %s %s", pKey, pfield, pValue);
    bool ret = false;
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_INTEGER)
        {
            ret = true;
            if (pReply->integer == 1)
                LOG_DEBUG("RedisCMD::HSET success key=%s,field=%s", pKey, pfield);
            else if (pReply->integer == 0)
                LOG_DEBUG("RedisCMD::HSET success:modify key=%s,field=%s", pKey, pfield);
        }
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            LOG_ERROR("RedisCMD::HSET error pReply->str=%s", pReply->str);
        }
    }

    if (ret == false)
        LOG_ERROR("RedisCMD::HSET failure key=%s", pKey);

    freeReplyObject(pReply);
    return ret;
}

string RedisCMD::hget(const char *pKey, const char *pfield)
{
    if (RedisCMD::ErrorCheck("HGET") == false)
        return "";

    redisReply *pReply = (redisReply *)redisCommand(s_pContext, "HGET %s %s", pKey, pfield);
    string ret;
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_STRING)
        {
            ret = pReply->str;
            LOG_DEBUG("RedisCMD::HGET success key=%s,field=%s,value=%s", pKey, pfield, ret.c_str());
        }
        else if (pReply->type == REDIS_REPLY_NIL)
        {
            LOG_ERROR("RedisCMD::HGET key=%s,field=%s value is null", pKey, pfield);
        }
        else if (pReply->type == REDIS_REPLY_ERROR)
        {
            LOG_ERROR("RedisCMD::HGET error key=%s,field=%s pReply->str=%s", pKey, pfield, pReply->str);
        }

        freeReplyObject(pReply);
    }
    else
    {
        LOG_ERROR("RedisCMD::HGET failure key=%s,field=%s", pKey, pfield);
    }

    return std::move(ret);
}