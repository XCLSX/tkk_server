#include "croommanger.h"

CRoomManger::CRoomManger()
{
   pthread_mutex_init(&lock,NULL);
}

bool CRoomManger::joinRoom(int room_id,int user_id)
{
    STRU_USERINROOM_ID*sui = map_uInr[room_id];
    pthread_mutex_lock(&lock);
    if(sui->num==5)
        return false;
    for(int i=0;i<5;i++)
    {
        if(sui->idarr[i]==0)
        {
            sui->idarr[i] = user_id;
            sui->num++;
            pthread_mutex_unlock(&lock);
            return true;
        }
    }
}

bool CRoomManger::CreateRoom(int room_id,int user_id)
{
    STRU_USERINROOM_ID *sui = new STRU_USERINROOM_ID;
    sui->idarr[sui->num++] =user_id;
    map_uInr[room_id] = sui;
    return true;
}



