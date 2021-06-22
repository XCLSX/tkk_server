#ifndef HERO_DEF
#define HERO_DEF
#include <packdef.h>

class Hero{

public:
    Hero()
    {

    }
protected:
    int hp;
    int hero_id;
    virtual void fun1(STRU_CARD *cardarr,int clen,int *userIdarr,int len) = 0;
    virtual void fun2(STRU_CARD *cardarr,int clen,int *userIdarr,int len) = 0;

};



#endif
