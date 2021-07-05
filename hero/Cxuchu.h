#ifndef CXUCHU_H
#define CXUCHU_H
#include <hero.h>
class Cxuchu :public Hero
{
public:
    Cxuchu()
    {
        hp = 4;
        country = wei;
        sex = 1;
    }
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
    void fun2(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
};
#endif // CXUCHU_H
