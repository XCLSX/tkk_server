#include "player.h"



player::player()
{
    this->m_hero_id = -1;
    alive = true;

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
    case caocao:
        m_hero = new Ccaocao;
        break;
    case diaochan:
        m_hero = new Cdiaochan;
        break;
    case guanyu:
        m_hero = new Cguanyu;
        break;
    case guojia:
        m_hero = new Cguojia;
        break;
    case huangyueying:
        m_hero = new Chuangyueying;
        break;
    case huatuo:
        m_hero = new Chuatuo;
        break;
    case liubei:
        m_hero = new Cliubei;
        break;
    case lvbu:
        m_hero = new Clvbu;
        break;
    case machao:
        m_hero = new Cmachao;
        break;
    case simayi:
        m_hero = new Csimayi;
        break;
    case sunquan:
        m_hero = new Csunquan;
        break;
    case xiahoudun:
        m_hero = new Cxiahoudun;
        break;
    case xuchu:
        m_hero = new Cxuchu;
        break;
    case zhangfei:
        m_hero = new Czhangfei;
        break;
    case zhangliao:
        m_hero =new Czhangliao;
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

void player::setwq(STRU_CARD *wq)
{
    if(wq == NULL)
        wuqi.id = 0;
    else
    wuqi = *wq;
}

void player::setfj(STRU_CARD * fj)
{
    if(fj == NULL)
        fangju.id = 0;
    else
    fangju = *fj;
}

void player::setjgm(STRU_CARD * jgm)
{
    if(jgm == NULL)
        jgma.id = 0;
    else
    jgma = *jgm;
}

void player::setfym(STRU_CARD * fym)
{
    if(fym == NULL)
        fyma.id = 0;
    else
    fyma = *fym;
}

STRU_CARD *player::getwq()
{

    return  &wuqi;
}

STRU_CARD *player::getfj()
{
    return &fangju;
}

STRU_CARD *player::getjgm()
{
    return &jgma;
}

STRU_CARD *player::getfym()
{
    return &fyma;
}

