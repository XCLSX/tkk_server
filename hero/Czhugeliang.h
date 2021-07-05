#ifndef CZHUGELIANG_H
#define CZHUGELIANG_H
#include <hero.h>
class Czhugeliang :public Hero
{
public:
    Czhugeliang()
    {
        hp = 3;
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
#endif // CZHUGELIANG_H
