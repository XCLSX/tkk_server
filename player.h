#ifndef PLAYER_H
#define PLAYER_H
#include <hero.h>
#include <Cliubei.h>
class player
{
public:
    player();
    vector<STRU_CARD*> m_CardBox;
    void Initplayer(int,int);
    void setHero(int);
    int m_iddentity;
    Hero* m_hero;
    int m_hero_id;
    int place;
};

#endif // PLAYER_H
