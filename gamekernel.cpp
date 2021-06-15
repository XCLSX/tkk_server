#include "gamekernel.h"

GameKernel::GameKernel()
{

}

void GameKernel::startGame(int room_id)
{
    CardManger *cm = new CardManger;
    cm->InitCard();
    cm->freshCard();
    m_map[room_id] = cm;
}
