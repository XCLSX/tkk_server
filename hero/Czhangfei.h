#ifndef CZHANGFEI_H
#define CZHANGFEI_H
#include<hero.h>
class Czhangfei :public Hero
{
public:
    Czhangfei()
    {
        hp = 4;
        country = shu;
        sex = 1;
    }
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
    void fun2(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
};
#endif // CZHANGFEI_H
