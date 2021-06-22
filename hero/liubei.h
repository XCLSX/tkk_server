#ifndef LIUBEI_H
#define LIUBEI_H
#include <hero.h>

class liubei :public Hero
{
public:
    liubei();
    void fun1(STRU_CARD *cardarr, int clen, int *userIdarr, int len) override;
};

#endif // LIUBEI_H
