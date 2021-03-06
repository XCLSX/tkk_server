#include "gamekernel.h"

GameKernel::GameKernel()
{
    give_up_wxkj_num = 0;
    gamestart = false;
    memset(idarr,0,sizeof(idarr));
    memset(readyarr,false,sizeof(readyarr));
    num = 0;
    totalHero = 17;
    //cardInit
    srand(time(NULL));
    memset(card,0,sizeof(card));
    pos = 0;
    current_index = 0;
    last_card_num = 0;
    off_card_num = 108;
    this->InitCard();

    pthread_mutex_init(&wxkj_lock,NULL);

    heroarr = new int[totalHero];
}

void GameKernel::InitPlayer(int user_id, int place)
{
    player * pl = new player();
    pl->place = place;
    map_idToplayer[user_id] = pl;
    wxkj_num = 0;
}

void GameKernel::nextTurn()
{
    while(1)
    {
        currentTurn++;
        if(currentTurn==5)
            currentTurn = 0;
        if(map_idToplayer[idarr[currentTurn]]->IsAlive())
        {
            break;
        }

    }
}

bool GameKernel::InitCard()
{
    setInfo(SHANDIAN,Spade1,1,YANSHIJINNANG);
    setInfo(JUEDOU,Spade1,1,FEIYANSHIJINNANG);
    setInfo(BAGUAZHEN,Spade1,2,FANGJU);
    setInfo(CIXIONGSHUANGGUJIAN,Spade1,2,WUQI);
    setInfo(GUOHECHAIQIAO,Spade1,3,FEIYANSHIJINNANG);
    setInfo(SHUNSHOUQIANYANG,Spade1,3,FEIYANSHIJINNANG);
    setInfo(GUOHECHAIQIAO,Spade1,4,FEIYANSHIJINNANG);
    setInfo(SHUNSHOUQIANYANG,Spade1,4,FEIYANSHIJINNANG);
    setInfo(QINGLONGYANYUEDAO,Spade1,5,WUQI);
    setInfo(JUEYING,Spade1,5,FANGYUMA);
    setInfo(LEBUSISHU,Spade1,6,YANSHIJINNANG);
    setInfo(QINGGANGJIAN,Spade1,6,WUQI);
    setInfo(NANMANRUQIN,Spade1,7,FEIYANSHIJINNANG);
    setInfo(SHA,Spade1,7,JICHU);
    setInfo(SHA,Spade1,8,JICHU);
    setInfo(SHA,Spade1,8,JICHU);
    setInfo(SHA,Spade1,9,JICHU);
    setInfo(SHA,Spade1,9,JICHU);
    setInfo(SHA,Spade1,10,JICHU);
    setInfo(SHA,Spade1,10,JICHU);
    setInfo(WUXIEKEJI,Spade1,11,FEIYANSHIJINNANG);
    setInfo(SHUNSHOUQIANYANG,Spade1,11,FEIYANSHIJINNANG);
    setInfo(ZHANGBASHEMAO,Spade1,12,WUQI);
    setInfo(GUOHECHAIQIAO,Spade1,12,FEIYANSHIJINNANG);
    setInfo(DAYUAN,Spade1,13,JINGONGMA);
    setInfo(NANMANRUQIN,Spade1,13,FEIYANSHIJINNANG);////////////////////////////////
    setInfo(TAOYUANJIEYI,Hearts2,1,FEIYANSHIJINNANG);
    setInfo(WANJIANQIFA,Hearts2,1,FEIYANSHIJINNANG);
    setInfo(SHAN,Hearts2,2,JICHU);
    setInfo(SHAN,Hearts2,2,JICHU);
    setInfo(TAO,Hearts2,3,JICHU);
    setInfo(WUGUFENGDENG,Hearts2,3,FEIYANSHIJINNANG);
    setInfo(TAO,Hearts2,4,JICHU);
    setInfo(WUGUFENGDENG,Hearts2,4,FEIYANSHIJINNANG);
    setInfo(QILINGONG,Hearts2,5,WUQI);
    setInfo(CHITU,Hearts2,5,JINGONGMA);
    setInfo(TAO,Hearts2,6,JICHU);
    setInfo(LEBUSISHU,Hearts2,6,YANSHIJINNANG);
    setInfo(TAO,Hearts2,7,JICHU);
    setInfo(WUZHONGSHENGYOU,Hearts2,7,FEIYANSHIJINNANG);
    setInfo(TAO,Hearts2,8,JICHU);
    setInfo(WUZHONGSHENGYOU,Hearts2,8,FEIYANSHIJINNANG);
    setInfo(TAO,Hearts2,9,JICHU);
    setInfo(WUZHONGSHENGYOU,Hearts2,9,FEIYANSHIJINNANG);
    setInfo(SHA,Hearts2,10,JICHU);
    setInfo(SHA,Hearts2,10,JICHU);
    setInfo(SHA,Hearts2,11,JICHU);
    setInfo(WUZHONGSHENGYOU,Hearts2,11,FEIYANSHIJINNANG);
    setInfo(TAO,Hearts2,12,JICHU);
    setInfo(GUOHECHAIQIAO,Hearts2,12,FEIYANSHIJINNANG);
    setInfo(ZHUAHUANGFEIDIAN,Hearts2,13,FANGYUMA);
    setInfo(SHAN,Hearts2,13,JICHU);//////////////////////////////////
    setInfo(ZHUGELIANNU,Club3,1,WUQI);
    setInfo(JUEDOU,Club3,1,FEIYANSHIJINNANG);
    setInfo(SHA,Club3,2,JICHU);
    setInfo(BAGUAZHEN,Club3,2,FANGJU);
    setInfo(GUOHECHAIQIAO,Club3,3,FEIYANSHIJINNANG);
    setInfo(SHA,Club3,3,JICHU);
    setInfo(GUOHECHAIQIAO,Club3,4,FEIYANSHIJINNANG);
    setInfo(SHA,Club3,4,JICHU);
    setInfo(DILU,Club3,5,FANGYUMA);
    setInfo(SHA,Club3,5,JICHU);
    setInfo(LEBUSISHU,Club3,6,YANSHIJINNANG);
    setInfo(SHA,Club3,6,JICHU);
    setInfo(NANMANRUQIN,Club3,7,FEIYANSHIJINNANG);
    setInfo(SHA,Club3,7,JICHU);
    setInfo(SHA,Club3,8,JICHU);
    setInfo(SHA,Club3,8,JICHU);
    setInfo(SHA,Club3,9,JICHU);
    setInfo(SHA,Club3,9,JICHU);
    setInfo(SHA,Club3,10,JICHU);
    setInfo(SHA,Club3,10,JICHU);
    setInfo(SHA,Club3,11,JICHU);
    setInfo(SHA,Club3,11,JICHU);
    setInfo(JIEDAOSHAREN,Club3,12,FEIYANSHIJINNANG);
    setInfo(WUXIEKEJI,Club3,12,FEIYANSHIJINNANG);
    setInfo(JIEDAOSHAREN,Club3,13,FEIYANSHIJINNANG);
    setInfo(WUXIEKEJI,Club3,13,FEIYANSHIJINNANG);///////////////////////
    setInfo(ZHUGELIANNU,Diamond4,1,WUQI);
    setInfo(JUEDOU,Diamond4,1,FEIYANSHIJINNANG);
    setInfo(SHAN,Diamond4,2,JICHU);
    setInfo(SHAN,Diamond4,2,JICHU);
    setInfo(SHAN,Diamond4,3,JICHU);
    setInfo(SHUNSHOUQIANYANG,Diamond4,3,FEIYANSHIJINNANG);
    setInfo(SHAN,Diamond4,4,JICHU);
    setInfo(SHUNSHOUQIANYANG,Diamond4,4,FEIYANSHIJINNANG);
    setInfo(SHAN,Diamond4,5,JICHU);
    setInfo(GUANSHIFU,Diamond4,5,WUQI);
    setInfo(SHAN,Diamond4,6,JICHU);
    setInfo(SHA,Diamond4,6,JICHU);
    setInfo(SHAN,Diamond4,7,JICHU);
    setInfo(SHA,Diamond4,7,JICHU);
    setInfo(SHAN,Diamond4,8,JICHU);
    setInfo(SHA,Diamond4,8,JICHU);
    setInfo(SHAN,Diamond4,9,JICHU);
    setInfo(SHA,Diamond4,9,JICHU);
    setInfo(SHAN,Diamond4,10,JICHU);
    setInfo(SHA,Diamond4,10,JICHU);
    setInfo(SHAN,Diamond4,11,JICHU);
    setInfo(SHAN,Diamond4,11,JICHU);
    setInfo(TAO,Diamond4,12,JICHU);
    setInfo(FANGTIANHUAJI,Diamond4,12,WUQI);
    setInfo(SHA,Diamond4,13,JICHU);
    setInfo(ZIXIN,Diamond4,13,FANGYUMA);////////////////////////////
    setInfo(HANBINGJIAN,Spade1,2,WUQI);
    setInfo(RENWANGDUN,Club3,2,FANGJU);
    setInfo(SHANDIAN,Hearts2,12,YANSHIJINNANG);
    setInfo(GUANSHIFU,Diamond4,12,WUQI);
    freshCard();

}

void GameKernel::setInfo(int id, int color, int num, int type)
{
    if(pos>=108)
        return ;
    offcard[pos].id = id;
    offcard[pos].col = color;
    offcard[pos].num = num;
    offcard[pos].type = type;
    pos++;
}

STRU_CARD *GameKernel::getCard()
{
    STRU_CARD *m_card = &card[current_index++];

    last_card_num--;
    if(last_card_num == 0)
        freshCard();
    return m_card;
}

void GameKernel::FUN_OffCard(STRU_CARD *)
{
    offcard[off_card_num].id = card->id;
    offcard[off_card_num].col = card->col;
    offcard[off_card_num].num = card->num;
    offcard[off_card_num].type = card->type;
    off_card_num++;
}

void GameKernel::freshCard()
{
    for(int i=0;i<off_card_num;i++)
    {
        int index = rand()%(off_card_num-i);

        card[off_card_num-1-i].id = offcard[index].id;
        card[off_card_num-1-i].col = offcard[index].col;
        card[off_card_num-1-i].num = offcard[index].num;
        card[off_card_num-1-i].type = offcard[index].type;
        if(index != off_card_num-1-i)
        CardSwap(offcard,index,off_card_num-1-i);

        last_card_num ++;
    }
    off_card_num = 0;
}

void GameKernel::CardSwap(STRU_CARD *card_arr, int index, int endindex)
{
    card_arr[index].id = card_arr[endindex].id;
    card_arr[index].col = card_arr[endindex].col;
    card_arr[index].num = card_arr[endindex].num;
    card_arr[index].type = card_arr[endindex].type;
}

void GameKernel::FreshHeroArr(int *arr, int zhugongid, bool isZhuGong)
{
    if(isZhuGong)
    {
        int num = totalHero - 3;
        for(int i=0;i<num;i++)
            heroarr[i] = i;
        for(int i=0;i<num;i++)
        {
            int randomIndex = rand()%(num-i);
            if(randomIndex!=num-i)
            {
                int temp = heroarr[randomIndex];
                heroarr[randomIndex] = heroarr[num - i - 1];
                heroarr[num - i - 1] = temp;
            }
        }
        arr[0] = liubei;
        arr[1] = caocao;
        arr[2] = sunquan;
        arr[3] = heroarr[0];
        arr[4] = heroarr[1];
        arr[5] = heroarr[2];
    }
    else
    {
        int num = totalHero-1;
        for(int i=0;i<num;i++)
        {
            if(i == zhugongid)
                heroarr[i] = totalHero-1;
            else
                heroarr[i] = i;
        }
        for(int i=0;i<num;i++)
        {
            int randomIndex = rand()%(num-i);
            if(randomIndex!=(num-i))
            {
                int temp = heroarr[randomIndex];
                heroarr[randomIndex] = heroarr[num - i - 1];
                heroarr[num - i - 1] = temp;
            }
        }
        for(int i=0;i<16;i++)
        {
            arr[i] = heroarr[i];
        }
    }

}

void GameKernel::Freshidentity(int *arr, int len)
{
    if(len == 5)
    {
        arr[0] = zhugong;
        arr[1] = zhongchen;
        arr[2] = fanzei;
        arr[3] = fanzei;
        arr[4] = neijian;
        for(int i=0;i<5;i++)
        {
            int randIndex = rand()%(5-i);
            if(randIndex!= 4-i)
            {
                arr[randIndex] = arr[randIndex]^arr[4-i];
                arr[4-i] = arr[randIndex]^arr[4-i];
                arr[randIndex] = arr[randIndex]^arr[4-i];
            }
        }
    }
}
//#define POST_CARD_FAIL          0
//#define POST_CARD_SUCCESS       1
//#define WAIT_POST_CARD          2
//#define SUCCESS_ALREAYD_KILL    3
//#define POST_CARD_CONTINUE      4
//#define GHCQ_SUCCESS            5
//#define SSQY_SUCCESS            6
void GameKernel::DealCard(int sockfd,char *buf)
{
    STRU_POSTCARD_RQ *rq = (STRU_POSTCARD_RQ*)buf;
        //Hilight(rq->m_touser1id);
        current_jnp = rq->m_card;
        isUsed = true;
        wxkj_arr.clear();
        currenet_id = rq->m_userid;
    //???????????????
    FUN_OffCard(&rq->m_card);
    //????????????
    auto ite = map_idToplayer[rq->m_userid]->m_CardBox.begin();
            while(ite!=map_idToplayer[rq->m_userid]->m_CardBox.end())
            {
                if(IscardEuqal(*ite,&rq->m_card))
                {
                    map_idToplayer[rq->m_userid]->m_CardBox.erase(ite);
                    break;
                }
                ++ite;
            }
    STRU_POSTCARD_RS rs;
    rs.y_userid = rq->m_userid;
    switch (rq->m_card.id) {
    case SHA:
    {
         rs.m_lResult = WAIT_POST_CARD;
         Hilight(rq->m_touser1id);
    }
        break;
    case TAO:
    {
        rs.m_lResult = POST_CARD_SUCCESS;
        UpdateStatus(rs.y_userid,1);
    }break;
    case GUOHECHAIQIAO:
    {
        Hilight(rq->m_touser1id);
        wxkj_num = 0;
        rs.m_lResult = WAIT_POST_CARD;
        rs.y_userid = rq->m_touser1id;


    }break;
    case SHUNSHOUQIANYANG:
    {
            Hilight(rq->m_touser1id);
            rs.m_lResult = WAIT_POST_CARD;
            rs.y_userid = rq->m_touser1id;

    }break;
    case JUEDOU:
    {
        if(this->wxkj_num>0)
        {
            rs.m_lResult = WAIT_POST_CARD;
        }
        else
        {
            rs.m_lResult = WAIT_POST_CARD;
        }
    }break;
    case JIEDAOSHAREN:
    {

    }break;
    case WUZHONGSHENGYOU:
    {
        rs.m_lResult = WAIT_POST_CARD;


    }break;
    case WANJIANQIFA:
    {
        nextTurn();
    }break;
    case NANMANRUQIN:
    {

        nextTurn();
        //tarPos = currentTurn;
        //Hilight(idarr[currentTurn]);
        STRU_POSTCARD_RQ rq1;
        rq1.isShow = false;
        rq1.m_card = rq->m_card;
        rq1.m_touser1id = idarr[currentTurn];
        for(int i=0;i<5;i++)
        {
            int sockfd = map_sockfd[idarr[i]];
            m_tcp->SendData(sockfd,(char *)&rq1,sizeof(rq1));
        }


    }break;
    case TAOYUANJIEYI:
    {
        nextTurn();
        STRU_POSTCARD_RQ rq1;
        rq1.isShow = false;
        rq1.m_card.id = WUXIEKEJI;



    }break;
    case WUGUFENGDENG:
    {
        nextTurn();

    }break;
    case SHANDIAN:
    {

    }break;
    case LEBUSISHU:
    {

    }break;
    case HANBINGJIAN:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }
    case CIXIONGSHUANGGUJIAN:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case QINGLONGYANYUEDAO:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case QINGGANGJIAN:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case ZHANGBASHEMAO:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case QILINGONG:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case ZHUGELIANNU:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case GUANSHIFU:
    {
        map_idToplayer[rq->m_userid]->setwq(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case BAGUAZHEN:
    {
        map_idToplayer[rq->m_userid]->setfj(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case RENWANGDUN:
    {
        map_idToplayer[rq->m_userid]->setfj(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case CHITU:
    {
        map_idToplayer[rq->m_userid]->setjgm(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case DAYUAN:
    {
        map_idToplayer[rq->m_userid]->setjgm(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case DILU:
    {
        map_idToplayer[rq->m_userid]->setfym(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case JUEYING:
    {
        map_idToplayer[rq->m_userid]->setfym(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case ZHUAHUANGFEIDIAN:
    {
        map_idToplayer[rq->m_userid]->setfym(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    case ZIXIN:
    {
        map_idToplayer[rq->m_userid]->setfym(&rq->m_card);
        rs.m_lResult = POST_CARD_SUCCESS;
    }break;
    default:
        return ;
        break;
    }
    m_tcp->SendData(sockfd,(char *)&rs,sizeof(rs));

}

void GameKernel::ResposeCard(int sockfd, char *buf)
{
    STRU_POSTCARD_RS_S *rs = (STRU_POSTCARD_RS_S *)buf;
    player *pl = map_idToplayer[rs->user_id];
    if(rs->m_lResult == post_failed)
    {
        switch (rs->y_card.id)
        {
            case SHA:
            {
                UpdateStatus(rs->user_id,-1);
                if(map_idToplayer[rs->user_id]->IsAlive())
                {
                    Hilight(rs->y_user_id);
                    STRU_POSTCARD_RS srs;
                    srs.m_lResult = POST_CARD_CONTINUE;
                    int sofd = map_sockfd[rs->y_user_id];
                    m_tcp->SendData(sofd,(char *)&srs,sizeof(srs));
                }
                else
                {
                    HealRq(rs->user_id,rs->y_user_id);
                }
            }
            break;
            case NANMANRUQIN:
            {
               UpdateStatus(rs->user_id,-1);
               nextTurn();
               if(currentTurn == rs->y_user_id)
               {
                   STRU_POSTCARD_RS rs1;
                   rs1.m_lResult = POST_CARD_CONTINUE;
                   rs1.y_userid = rs->y_user_id;

                   m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&rs1,sizeof(rs1));
                   return ;
               }
               STRU_POSTCARD_RQ rq1;
               rq1.isShow = false;
               rq1.m_card = rs->y_card;
               rq1.m_touser1id = idarr[currentTurn];
               m_tcp->SendData(map_sockfd[idarr[currentTurn]],(char *)&rq1,sizeof(rq1));

            }break;
            case WANJIANQIFA:
            {
                nextTurn();
                STRU_POSTCARD_RQ rq;
                rq.isShow = false;
                rq.m_card.id = WUXIEKEJI;

                {
                    //updateHp(rs->user_id,-1,gk);
                    nextTurn();
                    //???????????????
                    if(idarr[currentTurn]==rs->y_user_id)
                    {
                        STRU_POSTCARD_RS srs;
                        srs.m_lResult = POST_CARD_CONTINUE;
                        m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&srs,sizeof(srs));
                        return ;
                    }
                    STRU_POSTCARD_RQ srq;
                    srq.isShow = false;
                    srq.m_card = rs->y_card;
                    srq.m_userid = rs->y_user_id;
                    srq.m_touser1id = idarr[currentTurn];
                    for(int i=0;i<5;i++)
                    {
                        m_tcp->SendData(map_sockfd[idarr[i]],(char *)&srq,sizeof(srq));
                    }
                }


            }break;
            case WUXIEKEJI:
            {
               pthread_mutex_lock(&wxkj_lock);
               wxkj_arr.insert(sockfd);
               pthread_mutex_unlock(&wxkj_lock);
               if(wxkj_arr.size() == 4)
                   if(isUsed)
                   {
                       Hilight(rs->y_user_id);
                       switch (current_jnp.id) {
                       case GUOHECHAIQIAO:
                       {
                            STRU_POSTCARD_RS sprs;
                            //sprs.y_userid =
                       }
                       break;
                       case WUZHONGSHENGYOU:
                       {
                           STRU_GETCARD_RS card_rs;
                           card_rs.m_card[0] = *getCard();
                           card_rs.m_card[1] = *getCard();
                           m_tcp->SendData(map_sockfd[currenet_id],(char *)&card_rs,sizeof(card_rs));
                           STRU_POSTCARD_RS post_rs;
                           post_rs.m_lResult = POST_CARD_CONTINUE;
                           m_tcp->SendData(map_sockfd[currenet_id],(char *)&post_rs,sizeof(post_rs));
                       }break;
                       default:
                           break;
                       }
                   }

            }break;
            case GUOHECHAIQIAO:
            {
                pthread_mutex_lock(&wxkj_lock);
                wxkj_arr.insert(sockfd);
                pthread_mutex_unlock(&wxkj_lock);
                if(wxkj_arr.size() == 4)
                {
                    Hilight(rs->y_user_id);
                    STRU_GHCQ_RQ gh_rq;
                    gh_rq.m_userid = rs->y_user_id;
                    gh_rq.y_userid = rs->user_id;
                    player *ghpl = map_idToplayer[rs->user_id];
                    for(int i=0;i<ghpl->m_CardBox.size();i++)
                    {
                        gh_rq.m_card[i] = *ghpl->m_CardBox[i];
                    }
                    gh_rq.wq = *ghpl->getwq();
                    gh_rq.fj = *ghpl->getfj();
                    gh_rq.jgm = *ghpl->getjgm();
                    gh_rq.fym = *ghpl->getfym();
                    m_tcp->SendData(map_sockfd[gh_rq.m_userid],(char *)&gh_rq,sizeof(gh_rq));
                    STRU_POSTCARD_RS rs1;
                    rs1.m_lResult = POST_CARD_CONTINUE;
                    rs1.y_userid = rs->y_user_id;

                    m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&rs1,sizeof(rs1));
                    return ;
                }
                return ;
            }break;
            case SHUNSHOUQIANYANG:
            {
                pthread_mutex_lock(&wxkj_lock);
                wxkj_arr.insert(sockfd);
                pthread_mutex_unlock(&wxkj_lock);
                if(wxkj_arr.size() == 4)
                {
                    Hilight(rs->y_user_id);
                    STRU_SSQY_RQ ss_rq;
                    ss_rq.m_userid = rs->y_user_id;
                    ss_rq.y_userid = rs->user_id;
                    player *ghpl = map_idToplayer[rs->user_id];
                    for(int i=0;i<ghpl->m_CardBox.size();i++)
                    {
                        ss_rq.m_card[i] = *ghpl->m_CardBox[i];
                    }
                    ss_rq.wq = *ghpl->getwq();
                    ss_rq.fj = *ghpl->getfj();
                    ss_rq.jgm = *ghpl->getjgm();
                    ss_rq.fym = *ghpl->getfym();
                    m_tcp->SendData(map_sockfd[ss_rq.m_userid],(char *)&ss_rq,sizeof(ss_rq));
                    STRU_POSTCARD_RS rs1;
                    rs1.m_lResult = POST_CARD_CONTINUE;
                    rs1.y_userid = rs->y_user_id;

                    m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&rs1,sizeof(rs1));
                    return ;
                }
            }break;
            case WUZHONGSHENGYOU:
            {
                pthread_mutex_lock(&wxkj_lock);
                wxkj_arr.insert(sockfd);
                pthread_mutex_unlock(&wxkj_lock);
                if(wxkj_arr.size() == 4)
                {
                    STRU_GETCARD_RS card_rs;
                    card_rs.m_card[0] = *getCard();
                    card_rs.m_card[1] = *getCard();
                    m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&card_rs,sizeof(card_rs));
                    STRU_POSTCARD_RS post_rs;
                    post_rs.m_lResult = POST_CARD_CONTINUE;
                    m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&post_rs,sizeof(post_rs));
                }
            }break;
            default:
                break;
         }

    }
    else
    {

        //????????????
        STRU_POSTCARD_RQ rq;
        rq.m_card.id = rs->m_card.id;
        rq.m_card.col = rs->m_card.col;
        rq.m_card.num = rs->m_card.num;
        rq.m_card.type = rs->m_card.type;
        rq.m_userid = rs->user_id;


        for(int i=0;i<5;i++)
        {
            if(idarr[i] == rq.m_userid)
                continue;
            int sockfd = map_sockfd[idarr[i]];
            m_tcp->SendData(sockfd,(char *)&rq,sizeof(rq));

        }
        switch (rs->y_card.id) {
            case SHA:
            {
                Hilight(rs->y_user_id);
                STRU_POSTCARD_RS srs;
                srs.m_lResult = POST_CARD_CONTINUE;
                m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&srs,sizeof(srs));
            }break;
            case NANMANRUQIN:
            {
                if(rs->m_card.id == SHA)
                {
                   nextTurn();
                   //???????????????
                   if(idarr[currentTurn]==rs->y_user_id)
                   {
                       STRU_POSTCARD_RS srs;
                       srs.m_lResult = POST_CARD_CONTINUE;
                       m_tcp->SendData(map_sockfd[rs->y_user_id],(char *)&srs,sizeof(srs));
                       return ;
                   }
                   STRU_POSTCARD_RQ rq1;
                   rq1.isShow = false;
                   rq1.m_card = rs->y_card;
                   rq1.m_touser1id = idarr[currentTurn];
                   for(int i=0;i<5;i++)
                   {
                       int sockfd = map_sockfd[idarr[i]];
                       m_tcp->SendData(sockfd,(char *)&rq1,sizeof(rq1));
                   }
                   return ;
                }
                else if(rs->m_card.id == WUXIEKEJI)
                {
                    current_jnp = rs->y_card;
                    currenet_id = idarr[currentTurn];
                    wxkj_num = 0;

                }



            }break;
            case GUOHECHAIQIAO:
            {
                current_jnp = rs->y_card;
                isUsed = !isUsed;

            }break;
            case SHUNSHOUQIANYANG:
            {
                current_jnp = rs->y_card;
                isUsed = !isUsed;

            }break;
            case WUXIEKEJI:
            {
                pthread_mutex_lock(&wxkj_lock);
                isUsed = !isUsed;
                wxkj_num = 0;
                pthread_mutex_unlock(&wxkj_lock);
            }break;
            case WUZHONGSHENGYOU:
            {
                pthread_mutex_lock(&wxkj_lock);
                isUsed = !isUsed;
                wxkj_arr.clear();
                pthread_mutex_unlock(&wxkj_lock);
            }break;

        }

    }
}

bool GameKernel::CheckCard(STRU_CARD *card, int user_id)
{
    for(auto ite:map_idToplayer[user_id]->m_CardBox)
    {
      if(IscardEuqal(ite,card))
          return true;
    }
    return false;
}

bool GameKernel::IscardEuqal(STRU_CARD *card1, STRU_CARD *card2)
{
    if(card1->id == card2->id&&card1->col == card2->col&&card1->num==card2->num&&card1->type == card2->type)
        return true;
    return false;
}

void GameKernel::Hilight(int userid)
{
    if(userid == 0)
        return ;
    STRU_HILIGHT_RQ rq;
    rq.m_userid = userid;
    for(int i=0;i<5;i++)
    {
        int sockfd = map_sockfd[idarr[i]];
        m_tcp->SendData(sockfd,(char *)&rq,sizeof(rq));
    }
}

void GameKernel::UpdateStatus(int user_id, int hp_change, int card_change)
{
    STRU_COMMIT_STATUS scs;
    scs.user_id = user_id;

    scs.hp_change = map_idToplayer[user_id]->changeHp(hp_change);
    for(int i=0;i<5;i++)
    {
        int sockfd = map_sockfd[idarr[i]];
        m_tcp->SendData(sockfd,(char *)&scs,sizeof(scs));
    }
}

void GameKernel::HealRq(int user_id,int o_userid)
{
    STRU_HEAL_PLAYER_RQ rq;
    rq.die_userid = user_id;
    SetCurrentTurn(user_id);
    die_userid = user_id;
    cause_dieuserid = o_userid;
    Hilight(idarr[currentTurn]);
    m_tcp->SendData(map_sockfd[idarr[currentTurn]],(char *)&rq,sizeof(rq));

}

void GameKernel::HealRs(char* szbuf)
{
    STRU_HEAL_PLAYER_RS *rs = (STRU_HEAL_PLAYER_RS *)szbuf;
    if(rs->n_lResult)
    {
        UpdateStatus(die_userid,1,0);
        Hilight(idarr[temp_turn]);
        currentTurn = temp_turn;
        STRU_POSTCARD_RS prs;
        prs.m_lResult = POST_CARD_CONTINUE;
        m_tcp->SendData(map_sockfd[idarr[temp_turn]],(char *)&prs,sizeof(prs));
    }
    else
    {
        nextTurn();
        if(idarr[currentTurn] == die_userid)
        {
            //????????????
            STRU_PLAYER_DIE pd;
            pd.iddentity = map_idToplayer[rs->die_userid]->GetIddentity();
            pd.die_userid = rs->die_userid;
            for(int i=0;i<5;i++)
            {
                int sockfd = map_sockfd[idarr[i]];
                if(sockfd == pd.die_userid)
                    continue;
                m_tcp->SendData(sockfd,(char *)&pd,sizeof(pd));
                if(pd.iddentity == fanzei)
                {
                    STRU_GETCARD_RS grs;
                    for(int i=0;i<3;i++)
                    grs.m_card[i] = *getCard();
                    m_tcp->SendData(map_sockfd[cause_dieuserid],(char *)&grs,sizeof(grs));
                }
            }
            STRU_POSTCARD_RS prs;
            prs.m_lResult = POST_CARD_CONTINUE;
            m_tcp->SendData(map_sockfd[idarr[currentTurn]],(char *)&prs,sizeof(prs));

        }
        else
        {
            STRU_HEAL_PLAYER_RQ prq;
            prq.die_userid = rs->die_userid;
            Hilight(idarr[currentTurn]);
            m_tcp->SendData(map_sockfd[idarr[currentTurn]],(char *)&prq,sizeof(prq));
            Hilight(idarr[temp_turn]);
            currentTurn = temp_turn;

        }
    }

}

void GameKernel::SetCurrentTurn(int userid)
{
    temp_turn = currentTurn;
    while(1)
    {
        if(idarr[currentTurn] == userid)
            break;
        nextTurn();
    }
}

