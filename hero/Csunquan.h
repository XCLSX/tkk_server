#ifndef CSUNQUAN_H
#define CSUNQUAN_H
#include <hero.h>

class Csunquan :public Hero
{
public:
    Csunquan()
    {
        hp = 4;
        country = wu;
        sex = 1;
    }
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
    void fun2(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
};

#endif // CSUNQUAN_H
