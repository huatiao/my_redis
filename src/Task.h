#ifndef __REDIS_TASK_H__
#define __REDIS_TASK_H__

class Task
{
public:
    Task();
    ~Task();

    void RecvLoginReq(int iUID);  //handle the player login
    void RecvDisconnectReq(int iUID);//handle the player dicconnect
    void RecvOneGameEndReq(int iUID);//After the player has played a game

};


#endif //!__REDIS_TASK_H__