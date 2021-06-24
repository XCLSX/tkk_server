#ifndef HERO_DEF
#define HERO_DEF
#include <packdef.h>

class Hero{

public:
    Hero()
    {
    }
     ~Hero()
    {}
    int getHP()
    {
        return hp;
    }
    int getCountry()
    {
        return country;
    }
protected:
    int hp;
    int country;
    int sex;
    virtual void fun1(STRU_CARD *cardarr,int clen,int *userIdarr,int len) = 0;
    virtual void fun2(STRU_CARD *cardarr,int clen,int *userIdarr,int len) = 0;

};



#endif
