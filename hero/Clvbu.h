#ifndef CLVBU_H
#define CLVBU_H
#include <hero.h>
class Clvbu :public Hero
{
public:
    Clvbu()
    {
        hp = 4;
        country = qun;
        sex = 1;
    }
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
    void fun2(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
};
#endif // CLVBU_H
