#ifndef CGUOJIA_H
#define CGUOJIA_H
#include <hero.h>
class Cguojia :public Hero
{
public:
    Cguojia()
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
#endif // CGUOJIA_H
