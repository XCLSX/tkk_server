#ifndef CXIAHOUDUN_H
#define CXIAHOUDUN_H
#include <hero.h>
class Cxiahoudun :public Hero
{
public:
    Cxiahoudun()
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
#endif // CXIAHOUDUN_H
