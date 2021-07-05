#ifndef CHUANGYUEYING_H
#define CHUANGYUEYING_H
#include <hero.h>
class Chuangyueying :public Hero
{
public:
    Chuangyueying()
    {
        hp = 3;
        country = shu;
        sex = 0;
    }
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
    void fun2(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
};
#endif // CHUANGYUEYING_H
