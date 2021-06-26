#include "player.h"



player::player()
{

}

void player::Initplayer(int hero_id, int iddentity)
{
    m_hero_id = hero_id;
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
