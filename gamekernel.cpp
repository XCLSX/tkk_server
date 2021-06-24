#include "gamekernel.h"

GameKernel::GameKernel()
{
    memset(idarr,0,sizeof(idarr));
    memset(readyarr,false,sizeof(readyarr));
    num = 0;
}

void GameKernel::InitPlayer(int user_id, int iddentity, int hero_id)
{
    player * pl = new player(iddentity,hero_id);
    map_idToplayer[user_id] = pl;
}
