#ifndef CSIMAYI_H
#define CSIMAYI_H
#include <hero.h>
class Csimayi :public Hero
{
public:
    Csimayi()
    {
        hp = 3;
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
#endif // CSIMAYI_H
