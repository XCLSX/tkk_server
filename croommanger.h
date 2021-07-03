#ifndef CROOMMANGER_H
#define CROOMMANGER_H
#include <include/packdef.h>
#include <gamekernel.h>
#include <TCPNet.h>

class CRoomManger
{
public:
    CRoomManger();
    bool CreateRoom(int,int,int,TcpNet*);
    bool IsRoomexist(int );
    int joinRoom(int,int,int *,int);
    bool leaveRoom(int,int);        //false 房间所有人都退出  true 房间还存在其他人
public:
    pthread_mutex_t lock;
    map<int,GameKernel*>         map_gamekl;
    //map<int,CardManger*>         map_CardManger;
    //map<int,
};

#endif // CROOMMANGER_H
