#ifndef CZHAOYUN_H
#define CZHAOYUN_H
#include <hero.h>
class Czhaoyun :public Hero
{
public:
    Czhaoyun()
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
#endif // CZHAOYUN_H
