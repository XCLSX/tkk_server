#include "player.h"



player::player()
{
    this->m_hero_id = -1;
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
    switch (hero_id) {
    case liubei:
        m_hero = new Cliubei;
        break;
    default:
        m_hero = new Cliubei;
        break;
    }
}

void player::Heal()
{
    if(hp == full_hp)
        return ;
    hp++;
}
