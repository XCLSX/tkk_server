#ifndef CDIAOCHAN_H
#define CDIAOCHAN_H
#include<hero.h>
class Cdiaochan :public Hero
{
public:
    Cdiaochan()
    {
        hp = 3;
        country = qun;
        sex = 0;
    }
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
    void fun2(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override
    {

    }
};
#endif // CDIAOCHAN_H
