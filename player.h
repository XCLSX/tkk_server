#ifndef PLAYER_H
#define PLAYER_H
#include <hero.h>
#include <Cliubei.h>
class player
{
public:
    player();
    vector<STRU_CARD*> m_CardBox;
    void SetInfo(int hero_id,int iddentity);
    void setHero(int);
    int m_iddentity;
    Hero* m_hero;
    int hp;
    int full_hp;
    void Heal();
    int m_hero_id;
    int place;
    int wuqi;
    int fangju;
    int jgma;
    int fyma;
    int range;
};

#endif // PLAYER_H
