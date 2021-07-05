#ifndef CZHANGLIAO_H
#define CZHANGLIAO_H
#include <hero.h>
class Czhangliao :public Hero
{
public:
    Czhangliao()
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
#endif // CZHANGLIAO_H
