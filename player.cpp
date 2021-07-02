#include "player.h"



player::player()
{
    this->m_hero_id = -1;
    alive = true;
    wuqi = 0;
    fangju = 0;
    jgma = 0;
    fyma = 0;
    range = 1;
}



void player::SetInfo(int hero_id, int iddentity)
{
        m_hero_id = hero_id;
        m_iddentity = iddentity;
        setHero(hero_id);
        if(iddentity == zhugong)
            hp = m_hero->getHP()+1;
        else
            hp = m_hero->getHP();
        full_hp = hp;
}

void player::setHero(int hero_id)
{
    range = 1;
    switch (hero_id) {
    case liubei:
        m_hero = new Cliubei;
        break;
    default:
        m_hero = new Cliubei;
        break;
    }
}

int player::getHp()
{
    return this->hp;
}

int player::getRange()
{
    return this->range;
}

int player::changeHp(int hp_change)
{
    hp+=hp_change;
    return hp;
}

void player::die()
{
    alive = false;
}

bool player::IsAlive()
{
    return alive;
}

void player::setwq(int wq)
{
    wuqi = wq;
}

void player::setfj(int fj)
{
    fangju = fj;
}

void player::setjgm(int jgm)
{
    if(jgma == 0&&jgm!=0)
    {
        range++;
    }
    jgma = jgm;
}

void player::setfym(int fym)
{
    fyma = fym;
}

