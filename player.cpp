#include "player.h"



player::player(int iddentity,int hero_id)
{
    m_iddentity = iddentity;
    setHero(hero_id);
}

void player::setHero(int hero_id)
{
    switch (hero_id) {
    case liubei:
        m_hero = new Cliubei;
        break;
    default:
        break;
    }
}
