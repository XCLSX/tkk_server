#ifndef GAMEKERNEL_H
#define GAMEKERNEL_H
#include <packdef.h>
#include <time.h>
#include <player.h>
#include <TCPNet.h>
class GameKernel
{
public:
    GameKernel();
    int idarr[5];
    bool readyarr[5];
    map<int,int> map_sockfd;
    int num;
    void InitPlayer(int,int);
    int ZGplace;
    int currentTurn;
    TcpNet* m_tcp;
    map<int,player*> map_idToplayer;
    bool gamestart;
    void nextTurn();
public:
    bool InitCard();
    void setInfo(int,int,int,int);
    STRU_CARD *getCard();           //抽牌
    void FUN_OffCard(STRU_CARD*);
    void freshCard();
    void CardSwap(STRU_CARD*,int ,int);
    void FreshHeroArr(int *arr,int len,bool);  //洗武将牌
    void Freshidentity(int *arr,int len); //洗身份牌
    void DealCard(int sockfd,char *buf);
    void ResposeCard(char *buf);
    bool CheckCard(STRU_CARD *card,int user_id);
    bool IscardEuqal(STRU_CARD*,STRU_CARD*);
    int wxkj_num;
private:
    int pos;
    int current_index;
    int last_card_num;
    int off_card_num; //弃牌堆牌数
    STRU_CARD card[108];    //牌堆
    STRU_CARD offcard[108]; //弃牌堆
    int *heroarr;           //英雄牌堆
    int totalHero;




};

#endif // GAMEKERNEL_H
