#ifndef CHUATUO_H
#define CHUATUO_H
#include <hero.h>
class Chuatuo :public Hero
{
public:
    Chuatuo()
    {
        hp = 3;
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
#endif // CHUATUO_H
