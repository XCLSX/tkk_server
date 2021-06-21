#include "gamekernel.h"

GameKernel::GameKernel()
{
    srand(time(NULL));
}

void GameKernel::startGame(int room_id)
{
    CardManger *cm = new CardManger;
    cm->InitCard();
    cm->freshCard();
    m_map[room_id] = cm;
}

void GameKernel::Selidentity(int *arr, int len)
{
    if(len == 5)
    {
        arr[0] = zhugong;
        arr[1] = zhongchen;
        arr[2] = fanzei;
        arr[3] = fanzei;
        arr[4] = neijian;
        for(int i=0;i<5;i++)
        {
            int randIndex = rand()%5-i;
            if(randIndex!= 4-i)
            {
                arr[randIndex] = arr[randIndex]^arr[4-i];
                arr[4-i] = arr[randIndex]^arr[4-i];
                arr[randIndex] = arr[randIndex]^arr[4-i];
            }
        }
    }
}
