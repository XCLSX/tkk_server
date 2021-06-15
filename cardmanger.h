#ifndef CARDMANGER_H
#define CARDMANGER_H
#include <packdef.h>
#include <time.h>
class CardManger
{
public:
    CardManger();
    bool InitCard();
    void setInfo(int,int,int,int);
    STRU_CARD *getCard();
    void FUN_OffCard(STRU_CARD*);
    void freshCard();
    void CardSwap(int ,int);
private:
    int pos;
    int current_index;
    int last_card_num;
    int off_card_num; //弃牌堆牌数
    STRU_CARD card[108];    //牌堆
    STRU_CARD offcard[108]; //弃牌堆

};

#endif // CARDMANGER_H
