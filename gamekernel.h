#ifndef GAMEKERNEL_H
#define GAMEKERNEL_H
#include <cardmanger.h>

class GameKernel
{
public:
    GameKernel();
    void startGame(int);
    void Selidentity(int *arr,int len);
    map<int,CardManger*> m_map;
};

#endif // GAMEKERNEL_H
