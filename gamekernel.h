#ifndef GAMEKERNEL_H
#define GAMEKERNEL_H
#include <cardmanger.h>

class GameKernel
{
public:
    GameKernel();
    void startGame(int);

    map<int,CardManger*> m_map;
};

#endif // GAMEKERNEL_H
