#ifndef PLAYER_H
#define PLAYER_H
#include <Cliubei.h>
#include <Chuatuo.h>
#include <Ccaocao.h>
#include <Cdiaochan.h>
#include <Cguanyu.h>
#include <Cguojia.h>
#include <Chuangyueying.h>
#include <Clvbu.h>
#include <Cmachao.h>
#include <Csimayi.h>
#include <Csunquan.h>
#include <Cxiahoudun.h>
#include <Cxuchu.h>
#include <Czhangfei.h>
#include <Czhangliao.h>
#include <Czhaoyun.h>
#include <Czhugeliang.h>
class player
{
public:
    player();
    vector<STRU_CARD*> m_CardBox;
    void SetInfo(int hero_id,int iddentity);
    void setHero(int);
    int getHp();
    int getRange();
    int changeHp(int);
    void die();
    bool IsAlive();

    void setwq(STRU_CARD *);
    void setfj(STRU_CARD *);
    void setjgm(STRU_CARD *);
    void setfym(STRU_CARD *);
    int GetIddentity();
    STRU_CARD *getwq();
    STRU_CARD *getfj();
    STRU_CARD *getjgm();
    STRU_CARD *getfym();
    Hero* m_hero;
    int place;
    int m_hero_id;

private:
    int m_iddentity;
    int hp;
    int full_hp;
    bool alive;
    STRU_CARD wuqi;
    STRU_CARD fangju;
    STRU_CARD jgma;
    STRU_CARD fyma;
    int range;
};

#endif // PLAYER_H
