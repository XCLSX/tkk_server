#ifndef PLAYER_H
#define PLAYER_H
#include <hero.h>
#include <Cliubei.h>
class player
{
public:
    player(int,int);
    void setHero(int);
    vector<STRU_CARD*> m_CardBox;
    int m_iddentity;
    Hero* m_hero;
    int m_hero_id;
};

#endif // PLAYER_H
