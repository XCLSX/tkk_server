#ifndef GAMEKERNEL_H
#define GAMEKERNEL_H
#include <packdef.h>
#include <player.h>
class GameKernel
{
public:
    GameKernel();
    int idarr[5];
    bool readyarr[5];
    int num;
    void InitPlayer(int,int,int);
    map<int,player*> map_idToplayer;

};

#endif // GAMEKERNEL_H
