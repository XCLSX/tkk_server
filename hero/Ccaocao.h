#ifndef CCAOCAO_H
#define CCAOCAO_H
#include <hero.h>

class Ccaocao :public Hero
{
public:
    Ccaocao()
    {
        hp = 4;
        country = wei;
        sex = nan;
    }
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
    void fun2(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
};
#endif // CCAOCAO_H
