#include "croommanger.h"

CRoomManger::CRoomManger()
{
   pthread_mutex_init(&lock,NULL);

}

int CRoomManger::joinRoom(int room_id,int user_id)
{
    if(!IsRoomexist(room_id))
        return 0;
    GameKernel* gk = map_uInr[room_id];
    pthread_mutex_lock(&lock);
    if(gk->num==5)
        return 1;
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i]==0)
        {
            gk->idarr[i] = user_id;
            gk->num++;
            pthread_mutex_unlock(&lock);
            return 2;
        }
    }
}

bool CRoomManger::leaveRoom(int room_id,int user_id)
{
    pthread_mutex_lock(&lock);
    GameKernel* gk = map_uInr[room_id];
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i] == user_id)
        {
            gk->idarr[i] = 0;
            gk->num--;
            gk->readyarr[i] = false;
            break;
        }
    }
    if(gk->num==0)
    {
        auto ite = map_uInr.find(room_id);
        map_uInr.erase(ite);
        pthread_mutex_unlock(&lock);
        return false;
    }
    else
    {
        pthread_mutex_unlock(&lock);
        return true;
    }

}





bool CRoomManger::CreateRoom(int room_id,int user_id)
{
    GameKernel *gk = new GameKernel;
    gk->idarr[gk->num++] =user_id;
    map_uInr[room_id] = gk;
    CardManger *cm = new CardManger;
    map_CardManger[room_id] = cm;
    return true;
}

bool CRoomManger::IsRoomexist(int room_id)
{
    if(map_uInr.find(room_id)!=map_uInr.end())
        return true;
    return false;
}



