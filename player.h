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
    int getHp();
    int getRange();
    bool Heal();
    int DesHp();
    void die();
    bool IsAlive();

    void setwq(int);
    void setfj(int);
    void setjgm(int);
    void setfym(int);
    Hero* m_hero;
    int place;
    int m_hero_id;

private:
    int m_iddentity;
    int hp;
    int full_hp;
    bool alive;
    int wuqi;
    int fangju;
    int jgma;
    int fyma;
    int range;
};

#endif // PLAYER_H
