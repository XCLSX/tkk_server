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
    void FUN_OffCard(STRU_CARD*);   //弃牌
    void freshCard();
    void CardSwap(STRU_CARD*,int ,int);
    void FreshHeroArr(int *arr,int len,bool);  //洗武将牌
    void Freshidentity(int *arr,int len); //洗身份牌
    void DealCard(int sockfd,char *buf);
    void ResposeCard(int sockfd,char *buf);
    bool CheckCard(STRU_CARD *card,int user_id);
    bool IscardEuqal(STRU_CARD*,STRU_CARD*);
    void UpdateStatus(int user_id,int hp_change = 0,int card_change = 0);
    int wxkj_num;
    STRU_CARD current_jnp;
    int currenet_id;
    bool isUsed;
    int tarPos;
    int give_up_wxkj_num;//放弃用无懈可击的人数
private:
    pthread_mutex_t wxkj_lock;
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
