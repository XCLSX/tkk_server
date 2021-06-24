#include "cardmanger.h"

CardManger::CardManger()
{
   srand(time(NULL));
   memset(card,0,sizeof(card));
   pos = 0;
   current_index = 0;
   last_card_num = 0;
   off_card_num = 108;
   totalHero = 17;
   heroarr = new int[totalHero];

}

bool CardManger::InitCard()
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





}

void CardManger::setInfo(int id, int color, int num, int type)
{
    if(pos>=108)
        return ;
    offcard[pos].id = id;
    offcard[pos].col = color;
    offcard[pos].num = num;
    offcard[pos].type = type;
    pos++;
}

STRU_CARD *CardManger::getCard()
{
    STRU_CARD *m_card = &card[current_index++];

    last_card_num--;
    if(last_card_num == 0)
        freshCard();
    return m_card;
}

void CardManger::FUN_OffCard(STRU_CARD *card)
{
    offcard[off_card_num].id = card->id;
    offcard[off_card_num].col = card->col;
    offcard[off_card_num].num = card->num;
    offcard[off_card_num].type = card->type;
    off_card_num++;
}

void CardManger::freshCard()
{
    for(int i=0;i<off_card_num;i++)
    {
        int index = rand()%off_card_num-i;
        if(offcard[index].id!=-1)
          {
            card[off_card_num-1-i].id = offcard[index].id;
            card[off_card_num-1-i].col = offcard[index].col;
            card[off_card_num-1-i].num = offcard[index].num;
            card[off_card_num-1-i].type = offcard[index].type;
            offcard[index].id = -1;
            last_card_num ++;

          }
        else
            i--;
    }
    off_card_num = 0;
}

void CardManger::FreshHeroArr(int *arr, int zhugongid, bool isZhuGong)
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

void CardManger::Freshidentity(int *arr, int len)
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



//void CardManger::CardSwap(int index, int tail)
//{
//    STRU_CARD temp;
//    temp.id = card[index].id;
//    temp.col = card[index].col;
//    temp.num = card[index].num;
//    temp.type = card[index].type;

//    card[index].id = card[tail].id;
//    card[index].col = card[tail].col;
//    card[index].num = card[tail].num;
//    card[index].type = card[tail].type;

//    card[tail].id = temp.id;
//    card[tail].col = temp.col;
//    card[tail].num = temp.num;
//    card[tail].type = temp.type;
//}
