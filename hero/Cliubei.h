#ifndef LIUBEI_H
#define LIUBEI_H
#include <hero.h>

class Cliubei :public Hero
{
public:
    Cliubei()
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

#endif // LIUBEI_H
