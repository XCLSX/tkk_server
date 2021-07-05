#ifndef CGUANYU_H
#define CGUANYU_H
#include <hero.h>
class Cguanyu :public Hero
{
public:
    Cguanyu()
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
#endif // CGUANYU_H
