#ifndef CMACHAO_H
#define CMACHAO_H
#include <hero.h>
class Cmachao :public Hero
{
public:
    Cmachao()
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
#endif // CMACHAO_H
