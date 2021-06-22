#ifndef PLAYER_H
#define PLAYER_H
#include <hero.h>

class player
{
public:
    player();
    void setHero(int);
    vector<STRU_CARD*> m_CardBox;
    Hero* m_hero;
};

#endif // PLAYER_H
