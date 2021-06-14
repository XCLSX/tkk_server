#ifndef CROOMMANGER_H
#define CROOMMANGER_H
#include <include/packdef.h>

typedef struct STRU_USERINROOM_ID
{
    STRU_USERINROOM_ID()
    {
        memset(idarr,0,sizeof(idarr));
        memset(readyarr,false,sizeof(readyarr));
        num = 0;
    }
    int  idarr[5];
    bool readyarr[5];
    int num;

}STRU_USERINROOM_ID;

class CRoomManger
{
public:
    CRoomManger();
    bool CreateRoom(int,int);
    bool IsRoomexist(int );
    int joinRoom(int,int);

public:
    pthread_mutex_t lock;
    map<int,STRU_USERINROOM_ID*> map_uInr;
};

#endif // CROOMMANGER_H