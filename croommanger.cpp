#include "croommanger.h"

CRoomManger::CRoomManger()
{
   pthread_mutex_init(&lock,NULL);
}

int CRoomManger::joinRoom(int room_id,int user_id)
{
    if(!IsRoomexist(room_id))
        return 0;
    STRU_USERINROOM_ID*sui = map_uInr[room_id];
    pthread_mutex_lock(&lock);
    if(sui->num==5)
        return 1;
    for(int i=0;i<5;i++)
    {
        if(sui->idarr[i]==0)
        {
            sui->idarr[i] = user_id;
            sui->num++;
            pthread_mutex_unlock(&lock);
            return 2;
        }
    }
}

bool CRoomManger::leaveRoom(int room_id,int user_id)
{
    pthread_mutex_lock(&lock);
    STRU_USERINROOM_ID*sui = map_uInr[room_id];
    for(int i=0;i<5;i++)
    {
        if(sui->idarr[i] == user_id)
        {
            sui->idarr[i] = 0;
            break;
        }
    }
    for(int i=0;i<5;i++)
    {
        if(sui->idarr[i]!=0)
        {
            pthread_mutex_unlock(&lock);
            return true;
        }
    }
    return false;

}

bool CRoomManger::CreateRoom(int room_id,int user_id)
{
    STRU_USERINROOM_ID *sui = new STRU_USERINROOM_ID;
    sui->idarr[sui->num++] =user_id;
    map_uInr[room_id] = sui;
    return true;
}

bool CRoomManger::IsRoomexist(int room_id)
{
    if(map_uInr.find(room_id)!=map_uInr.end())
        return true;
    return false;
}



