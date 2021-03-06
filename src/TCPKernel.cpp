#include<TCPKernel.h>
#include "packdef.h"
#include<stdio.h>

using namespace std;



static const ProtocolMap m_ProtocolMapEntries[] =
{
    {DEF_PACK_REGISTER_RQ , &TcpKernel::Register},
    {DEF_PACK_LOGIN_RQ , &TcpKernel::Login},
    {DEF_PACK_CREATEROOM_RQ,&TcpKernel::CreateRoom},
    {DEF_PACK_ASKROOM_RQ,&TcpKernel::AskRoom},
    {DEF_PACK_OFFLINRE_RQ,&TcpKernel::OffLine},
    {DEF_PACK_ADD_FRIEND_RQ,&TcpKernel::AddfriendRq},
    {DEF_PACK_ADD_FRIEND_RS,&TcpKernel::AddfriendRs},
    {DEF_PACK_SEARCH_FRIEND_RQ,&TcpKernel::SearchFriend},
    {DEF_ALTER_USERINFO_RQ,&TcpKernel::AlterUserInfo},
    {DEF_PACK_GETFRIENDLIST_RQ,&TcpKernel::GetFriList},
    {DEF_PACK_SEARCH_ROOM_RQ,&TcpKernel::SearchRoom},
    {DEF_PACK_JOINROOM_RQ,&TcpKernel::JoinRoom},
    {DEF_PACK_CHECKOFFLINE_RQ,&TcpKernel::CheckOfflineMsg},
    {DEF_PACK_LEAVEROOM_RQ,&TcpKernel::LeaveRoom},
    {DEF_PACK_STARTGAME_RQ,&TcpKernel::StartGame},
    {DEF_PACK_SELHERO_RS,&TcpKernel::SelHeroRs},
    {DEF_PACK_GETCARD_RQ,&TcpKernel::GetCard},
    {DEF_PACK_POSTCARD_RQ,&TcpKernel::PostCard},
    {DEF_PACK_POSTCARD_RS_S,&TcpKernel::ResposeCard},
    {DEF_PACK_OFFCARD_RQ,&TcpKernel::ChangeTurn},
    {DEF_PACK_SSQY_RS,&TcpKernel::SSQY_Rs},
    {DEF_PACK_GHCQ_RS,&TcpKernel::GHCQ_Rs},
    {0,0}
};

int TcpKernel::Open()
{
    m_sql = new CMysql;
    m_tcp = new TcpNet(this);
    m_tcp->SetpThis(m_tcp);
    pthread_mutex_init(&m_tcp->alock,NULL);
    pthread_mutex_init(&m_tcp->rlock,NULL);
    if(!m_sql->ConnectMysql("localhost","root","root","tkk"))
    {
        printf("Conncet Mysql Failed...\n");
        return FALSE;
    }
    else
    {
        printf("MySql Connect Success...\n");
    }
    if( !m_tcp->InitNetWork()  )
    {
        printf("InitNetWork Failed...\n");
        return FALSE;
    }
    else
    {
        printf("Init Net Success...\n");
    }
    ////***********************////
    m_RoomManger = new CRoomManger;
    char szsql[_DEF_SQLIEN] = {0};
    sprintf(szsql,"truncate table t_room;");
    if(!m_sql->UpdataMysql(szsql))
    {
        printf("sql error:%s\n",szsql);
    }

    return TRUE;
}

void TcpKernel::Close()
{

    m_sql->DisConnect();
    m_tcp->UnInitNetWork();
}

void TcpKernel::DealData(int clientfd,char *szbuf,int nlen)
{
    PackType *pType = (PackType*)szbuf;
    int i = 0;
    while(1)
    {
        if(*pType == m_ProtocolMapEntries[i].m_type)
        {
            PFUN fun= m_ProtocolMapEntries[i].m_pfun;
            (this->*fun)(clientfd,szbuf,nlen);
        }
        else if(m_ProtocolMapEntries[i].m_type == 0 &&
                m_ProtocolMapEntries[i].m_pfun == 0)
            return;
        ++i;
    }
    return;
}





//??????
void TcpKernel::Register(int clientfd,char* szbuf,int nlen)
{
    printf("clientfd:%d RegisterRq\n", clientfd);

    STRU_REGISTER_RQ * rq = (STRU_REGISTER_RQ *)szbuf;
    STRU_REGISTER_RS rs;
    char szsql[_DEF_SQLIEN] = {0};
    list<string> ls;
    snprintf(szsql,sizeof(szsql),"select user_account,user_emall from t_user where user_account='%s' or user_emall='%s';"
             ,rq->m_szUser,rq->m_szEmall);
    if(!m_sql->SelectMysql(szsql,2,ls))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    //??????????????????????????????
    if(ls.size()>0)
    {
        rs.m_lResult = userid_is_exist;
    }
    else
    {   //????????????????????????-> t_user
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"insert into t_user values (null,'%s','%s','%s');",rq->m_szUser,rq->m_szEmall,rq->m_szPassword);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        //??????????????????????????????
        bzero(szsql,sizeof(szsql));
        sprintf(szsql,"select user_id from t_user where user_account = '%s';",rq->m_szUser);
        if(!m_sql->SelectMysql(szsql,1,ls))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        snprintf(szsql,sizeof(szsql),"insert into t_userInfo values(%d,1,'%s',' ????????????????????????????????????????????????',0);"
                 ,atoi(ls.front().c_str()),rq->m_szUser);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }


        rs.m_lResult = register_sucess;
    }

    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}

//??????
void TcpKernel::Login(int clientfd ,char* szbuf,int nlen)
{
    printf("Login\n");

    STRU_LOGIN_RQ * rq = (STRU_LOGIN_RQ *)szbuf;
    STRU_LOGIN_RS rs;

    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"select user_id,user_pwd from t_user where user_account = '%s';"
             ,rq->m_szUser);
    if(!m_sql->SelectMysql(szsql,2,ls))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    if(ls.size()==0)
    {
        rs.m_lResult =userid_no_exist;
        m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
        return ;

    }
    int user_id = atoi(ls.front().c_str());     ls.pop_front();

    if(strcmp(ls.front().c_str(),rq->m_szPassword)==0)
    {
        //????????????
        rs.m_lResult = login_sucess;
        //???????????????????????? ????????????????????????1
        bzero(szsql,sizeof(szsql));     ls.clear();
        sprintf(szsql,"select pic_id,user_name,felling from t_userInfo where user_id = %d;",user_id);
        if(!m_sql->SelectMysql(szsql,3,ls))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"update t_userInfo set status = 1  where user_id = %d;"
                                     ,user_id);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        //?????????????????????????????????
        STRU_USER_INFO_S *userInfo = new STRU_USER_INFO_S;
        userInfo->sockfd = clientfd;
        userInfo->status = 1;
        userInfo->icon_id = atoi(ls.front().c_str());             ls.pop_front();
        strcpy(userInfo->m_szName,ls.front().c_str());            ls.pop_front();
        strcpy(userInfo->m_szFelling,ls.front().c_str());
        map_IdtoUserInfo[user_id] = userInfo;
        //?????????
        rs.m_userInfo.m_userid = user_id;
        rs.m_userInfo.status = 1;
        rs.m_userInfo.m_iconid = userInfo->icon_id;
        strcpy(rs.m_userInfo.m_szName,userInfo->m_szName);
        strcpy(rs.m_userInfo.m_szFelling,userInfo->m_szFelling);
    }
    else
           rs.m_lResult = password_error;

    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
    //??????????????????
}

//????????????
void TcpKernel::SearchFriend(int clientfd, char *szbuf, int nlen)
{
    printf("SearchFriend\n");
    STRU_SEARCH_FRIEND_RQ *rq = (STRU_SEARCH_FRIEND_RQ*)szbuf;
    STRU_SEARCH_FRIEND_RS  rs;
    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"select user_id,pic_id,user_name,felling,status from t_userInfo where user_name = '%s';",rq->sz_friendName);
    if(!m_sql->SelectMysql(szsql,5,ls))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    if(ls.size()==0)
    {
        //??????????????????
        rs.m_result = no_this_user;
        m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
        return ;

    }
    rs.m_friInfo.m_userid = atoi(ls.front().c_str());   ls.pop_front();
    rs.m_friInfo.m_iconid = atoi(ls.front().c_str());   ls.pop_front();
    strcpy(rs.m_friInfo.m_szName,ls.front().c_str());   ls.pop_front();
    strcpy(rs.m_friInfo.m_szFelling,ls.front().c_str());  ls.pop_front();
    rs.m_friInfo.status = atoi(ls.front().c_str());    ls.clear();

    //????????????????????????
    bzero(szsql,sizeof(szsql));
    snprintf(szsql,sizeof(szsql),"select user_id from t_friend where user_id = %d and friend_id = %d;"
             ,rq->m_userid,rs.m_friInfo.m_userid);
    if(!m_sql->SelectMysql(szsql,1,ls))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    if(ls.size()==0)
        rs.m_result = search_success;
    else
        rs.m_result = is_your_friend;
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );

}

//??????????????????
void TcpKernel::AddfriendRq(int clientfd, char *szbuf, int nlen)
{
    printf("Addfriend\n");

    STRU_ADD_FRIEND_RQ *rq = (STRU_ADD_FRIEND_RQ*)szbuf;
    //????????????
    if(map_IdtoUserInfo.find(rq->m_friInfo.m_userid)!=map_IdtoUserInfo.end())
    {
        m_tcp->SendData(map_IdtoUserInfo[rq->m_friInfo.m_userid]->sockfd,szbuf,nlen);
    }
    else
    {
        list<string> ls;
        char szsql[_DEF_SQLIEN] = {0};
        sprintf(szsql,"insert into t_addfriend values(%d,%d);",rq->m_friInfo.m_userid,rq->m_userInfo.m_userid);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
    }
}

//??????????????????
void TcpKernel::AddfriendRs(int clientfd, char *szbuf, int nlen)
{
    STRU_ADD_FRIEND_RS *rs = (STRU_ADD_FRIEND_RS*)szbuf;
    if(rs->m_result == add_success)
    {

        char szsql[_DEF_SQLIEN] = {0};
        sprintf(szsql,"insert into t_friend values(%d,%d);",rs->m_userInfo.m_userid,rs->m_friInfo.m_userid);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }   bzero(szsql,sizeof(szsql));
        sprintf(szsql,"insert into t_friend values(%d,%d);",rs->m_friInfo.m_userid,rs->m_userInfo.m_userid);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        if(map_IdtoUserInfo.find(rs->m_userInfo.m_userid)!=map_IdtoUserInfo.end())
        {
            m_tcp->SendData(map_IdtoUserInfo[rs->m_userInfo.m_userid]->sockfd,szbuf,nlen);
        }
    }
}

//??????????????????
void TcpKernel::GetFriList(int clientfd ,char* szbuf,int nlen)
{
    STRU_GETFRILIST_RQ *rq = (STRU_GETFRILIST_RQ *)szbuf;
    STRU_GETFRILIST_RS rs;
    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    sprintf(szsql,"select friend_id,pic_id,user_name,felling,status from v_friend where user_id = %d;",rq->m_userid);
    if(!m_sql->SelectMysql(szsql,5,ls))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    int i=0;
    while(ls.size()!=0)
    {
        rs.m_friArr[i].m_userid = atoi(ls.front().c_str());          ls.pop_front();
        rs.m_friArr[i].m_iconid = atoi(ls.front().c_str());          ls.pop_front();
        strcpy(rs.m_friArr[i].m_szName,ls.front().c_str());          ls.pop_front();
        strcpy(rs.m_friArr[i].m_szFelling,ls.front().c_str());       ls.pop_front();
        rs.m_friArr[i].status = atoi(ls.front().c_str());            ls.pop_front();
        i++;
    }
    rs.m_lResult = ask_room_success;
    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));
}

//????????????
void TcpKernel::CreateRoom(int clientfd, char *szbuf, int nlen)
{
    printf("sockfd:%d\t????????????\n",clientfd);
    STRU_CREATEROOM_RQ *rq = (STRU_CREATEROOM_RQ*)szbuf;
    STRU_CREATEROOM_RS rs;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"insert into t_room values(null,'%s','%s');"
             ,rq->m_RoomName,map_IdtoUserInfo[rq->m_userid]->m_szName);
    if(!m_sql->UpdataMysql(szsql))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    list<string> ls;
    bzero(szsql,sizeof(szsql));
    snprintf(szsql,sizeof(szsql),"select room_id from t_room where  room_creator_name = '%s';"
             ,map_IdtoUserInfo[rq->m_userid]->m_szName);
    if(!m_sql->SelectMysql(szsql,1,ls))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    if(ls.size()==0)
        rs.m_lResult = create_failed;
    else
    {
        rs.m_lResult = create_success;
        rs.m_RoomId = atoi(ls.front().c_str());
        m_RoomManger->CreateRoom(clientfd,rs.m_RoomId,rq->m_userid,m_tcp);
    }
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}

//????????????
void TcpKernel::AskRoom(int clientfd ,char* szbuf,int nlen)
{
    printf("sockfd:%d\t????????????\n",clientfd);

    STRU_ASKROOM_RQ *rq = (STRU_ASKROOM_RQ*) szbuf;
    STRU_ASKROOM_RS rs;

    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"select room_id,room_name,room_creator_name from t_room;");
    if(!m_sql->SelectMysql(szsql,3,ls))
    {
        printf("sql error:%s\n",szsql);
        return ;
    }
    int i=0;
    if(ls.size()>0)
        rs.m_lResult = ask_room_success;
    else
        rs.m_lResult = ask_room_failed;
    while(ls.size()>0)
    {
        int room_id = atoi(ls.front().c_str());               ls.pop_front();
        rs.m_RoomList[i].m_Roomid = room_id;
        strcpy(rs.m_RoomList[i].sz_Roomname,ls.front().c_str());  ls.pop_front();
        strcpy(rs.m_RoomList[i].sz_RoomCreator,ls.front().c_str());   ls.pop_front();
        rs.m_RoomList[i].m_num = m_RoomManger->map_gamekl[room_id]->num;
        i++;
        if(i==MAX_ROOMLIST)
            break;
    }
    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));

}

//????????????
void TcpKernel::JoinRoom(int clientfd, char *szbuf, int nlen)
{

    STRU_JOINROOM_RQ *rq = (STRU_JOINROOM_RQ*)szbuf;
    STRU_JOINROOM_RS rs;
    printf("sockfd:%d\t??????%d?????????\n",clientfd,rq->m_RoomID);

    int relt = m_RoomManger->joinRoom(rq->m_RoomID,rq->m_userInfo.m_userid,&rs.place,map_IdtoUserInfo[rq->m_userInfo.m_userid]->sockfd);
    if(relt == 0)
        rs.m_lResult = room_no_exist;
    else if(relt == 1)
        rs.m_lResult = room_is_full;
    else
    {
         rs.m_lResult = join_success;
         GameKernel *gk = m_RoomManger->map_gamekl[rq->m_RoomID];
         gk->InitPlayer(rq->m_userInfo.m_userid,rs.place);
         m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));
         UpdateRoomMemberInfo(rq->m_RoomID);
    }
}

//????????????
void TcpKernel::StartGame(int clientfd, char *szbuf, int nlen)
{
    STRU_STARTGAME_RQ *rq = (STRU_STARTGAME_RQ *)szbuf;
    GameKernel *gk = m_RoomManger->map_gamekl[rq->Room_id];
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i]==rq->user_id)
        {
            gk->readyarr[i] = !gk->readyarr[i];
            break;
        }
    }
    for(int i=0;i<5;i++)
    {
        if(!gk->readyarr[i])
            return ;
    }
    STRU_STARTGAME_RS rs;
    rs.m_lResult = game_start_success;
    gk->gamestart = true;
    for(int i=0;i<5;i++)
    {
        int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
        m_tcp->SendData(sockfd,(char *)&rs,sizeof(rs));
    }

    //????????????
    int arr[5] = {0};
    //int arr[5] = {1,2,3,3,4};
    m_RoomManger->map_gamekl[rq->Room_id]->Freshidentity(arr,5);
    STRU_POST_IDENTITY spi;
    int ZGindex = 0;
    for(int i=0;i<5;i++)
        if(arr[i] == zhugong)
        {
            ZGindex = i;
            spi.m_ZG_userid = gk->idarr[i];
            gk->ZGplace = i;
            break;
        }

    for(int i=0;i<5;i++)
    {
        int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
        spi.m_identity = arr[i];
        spi.m_ZG_userid = gk->idarr[ZGindex];
        m_tcp->SendData(sockfd,(char*)&spi,sizeof(spi));
    }
   //???????????????
    SelHeroRq(gk->idarr[ZGindex],rq->Room_id,true,NULL);
}

//??????????????????
void TcpKernel::SelHeroRq(int id,int roomid,bool isZG,int *arr,int ZG_heroid)
{
    STRU_SELHERO_RQ rq;
    if(isZG)
    {
        m_RoomManger->map_gamekl[roomid]->FreshHeroArr(rq.m_HeroArr,0,isZG);
        int sockfd = map_IdtoUserInfo[id]->sockfd;
        m_tcp->SendData(sockfd,(char *)&rq,sizeof(rq));
    }
    else
    {
        int sockfd = map_IdtoUserInfo[id]->sockfd;
        for(int i=0;i<4;i++)
            rq.m_HeroArr[i] = arr[i];
        rq.ZG_heroid = ZG_heroid;
        m_tcp->SendData(sockfd,(char *)&rq,sizeof(rq));
    }
}

//??????????????????
void TcpKernel::SelHeroRs(int clientfd, char *szbuf, int nlen)
{
    STRU_SELHERO_RS *rs = (STRU_SELHERO_RS *)szbuf;
    GameKernel *gk = m_RoomManger->map_gamekl[rs->room_id];
    gk->map_idToplayer[rs->user_id]->SetInfo(rs->hero_id,rs->iddentity);
    if(rs->isZG)
    {
        STRU_SELHERO_RQ rq;
        int arr[16] = {0};
        int *parr = arr;
        m_RoomManger->map_gamekl[rs->room_id]->FreshHeroArr(arr,rs->hero_id,false);
       for(int i=0;i<5;i++)
       {
           if(gk->idarr[i] == rs->user_id)
               continue;
           SelHeroRq(gk->idarr[i],rs->room_id,false,parr,rs->hero_id);
           parr+=4;
       }
    }
    else
    {
        //???????????????????????????
        for(int i=0;i<5;i++)
            if(gk->map_idToplayer[gk->idarr[i]]->m_hero_id==-1)
                return ;

         //??????????????????
         AllSelHero(rs->room_id);
         //??????
         STRU_GETCARD_RQ cardrq;
         cardrq.m_roomid = rs->room_id;
         gk->currentTurn = gk->ZGplace;
         for(int i=0;i<5;i++)
         {
             cardrq.m_userid = gk->idarr[gk->currentTurn++];
             cardrq.num = 4;
             GetCard(map_IdtoUserInfo[cardrq.m_userid]->sockfd,(char *)&cardrq,sizeof(cardrq));
             if(gk->currentTurn==5)
                 gk->currentTurn = 0;
         }
         //??????????????????
         STRU_TURN_BEGIN tb;
         tb.user_id = gk->idarr[gk->currentTurn];
         for(int i=0;i<5;i++)
         {
             int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
             m_tcp->SendData(sockfd,(char *)&tb,sizeof(tb));
         }


    }
}

//??????????????????
void TcpKernel::AllSelHero(int roomid)
{
    GameKernel *gk = m_RoomManger->map_gamekl[roomid];
       STRU_ALLSEL_HERO_RS rs;
       for(int j = 0;j<5;j++)
       {
            int user_id = gk->idarr[j];
            player *temp = gk->map_idToplayer[user_id];
            rs.m_playerarr[j].user_id = user_id;
            rs.m_playerarr[j].hero_id = temp->m_hero_id;
            rs.m_playerarr[j].hp = temp->getHp();
            rs.m_playerarr[j].range = temp->getRange();
       }
       for(int i=0;i<5;i++)
       {
           int uid = gk->idarr[i];

           m_tcp->SendData(map_IdtoUserInfo[uid]->sockfd,(char *)&rs,sizeof(rs));

       }
}

//????????????
void TcpKernel::GetCard(int clientfd, char *szbuf, int nlen)
{
    STRU_GETCARD_RQ *rq = (STRU_GETCARD_RQ *)szbuf;
    GameKernel *gk = m_RoomManger->map_gamekl[rq->m_roomid];

    STRU_GETCARD_RS rs;
    for(int i=0;i<rq->num;i++)
    {
        STRU_CARD *tempcard = gk->getCard();
        gk->map_idToplayer[rq->m_userid]->m_CardBox.push_back(tempcard);
        if(tempcard->id == WUXIEKEJI)
            gk->wxkj_num++;
        rs.m_card[i].id = tempcard->id;
        rs.m_card[i].col = tempcard->col;
        rs.m_card[i].num = tempcard->num;
        rs.m_card[i].type = tempcard->type;
    }
    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));
}
//????????????
void TcpKernel::PostCard(int clientfd, char *szbuf, int nlen)
{
    STRU_POSTCARD_RQ *rq = (STRU_POSTCARD_RQ*)szbuf;
    //STRU_POSTCARD_RQ rq1;

    GameKernel* gk = m_RoomManger->map_gamekl[rq->m_roomid];
    //????????????????????????
//    if(!gk->CheckCard(&rq->m_card,rq->m_userid))
//    {
//        //?????????
//        STRU_POSTCARD_RS rs;
//        rs.m_lResult = POST_CARD_FAIL;
//        m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));
//        return ;
//    }
//    //??????????????????

    gk->DealCard(clientfd,szbuf);
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i] == rq->m_userid)
            continue;
        m_tcp->SendData(map_IdtoUserInfo[gk->idarr[i]]->sockfd,szbuf,nlen);
    }


}
//???????????????   ????????????
void TcpKernel::ResposeCard(int clientfd, char *szbuf, int nlen)
{
    STRU_POSTCARD_RS_S *rs = (STRU_POSTCARD_RS_S*) szbuf;
    GameKernel *gk  = m_RoomManger->map_gamekl[rs->room_id];
    gk->ResposeCard(clientfd,szbuf);
}

void TcpKernel::SSQY_Rs(int clientfd, char *szbuf, int nlen)
{
    STRU_SSQY_RS *rs = (STRU_SSQY_RS*)szbuf;
    GameKernel *gk = m_RoomManger->map_gamekl[rs->room_id];
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i] == rs->m_userid)
            continue;
        m_tcp->SendData(gk->map_sockfd[gk->idarr[i]],szbuf,nlen);
    }
    player *pl = gk->map_idToplayer[rs->m_userid];
    switch (rs->n_lResult) {
        case shoupai:
        {
            auto ite = pl->m_CardBox.begin();
            while(ite!=pl->m_CardBox.end())
            {
                if(gk->IscardEuqal(*ite,&rs->m_card))
                {
                    pl->m_CardBox.erase(ite);

                    return;
                }
                ++ite;
            }
        }
            break;
        case wqpai:
        {
            pl->setwq(0);
        }
        break;
        case fjpai:
        {
            pl->setfj(0);
        }
        break;
        case jgmpai:
        {
            pl->setjgm(0);
        }
        break;
        case fympai:
        {
            pl->setfym(0);
        }
        break;
        default:
            break;
    }
    STRU_GETCARD_RS card_rs;
    card_rs.m_card[0] = rs->m_card;
    m_tcp->SendData(map_IdtoUserInfo[rs->m_userid]->sockfd,(char *)&card_rs,sizeof(card_rs));
}

void TcpKernel::GHCQ_Rs(int clientfd, char *szbuf, int nlen)
{
    STRU_GHCQ_RS *rs = (STRU_GHCQ_RS*)szbuf;
    STRU_GHCQ_RS gh_rs;
    gh_rs.m_card = rs->m_card;
    gh_rs.m_userid = rs->m_userid;
    gh_rs.n_lResult = rs->n_lResult;
    gh_rs.room_id = rs->room_id;
    gh_rs.y_userid = rs->y_userid;
    GameKernel *gk = m_RoomManger->map_gamekl[rs->room_id];
    for(int i=0;i<5;i++)
    {
        if(map_IdtoUserInfo[gk->idarr[i]]->sockfd == clientfd)
            continue;
        m_tcp->SendData(gk->map_sockfd[gk->idarr[i]],(char *)&gh_rs,sizeof(gh_rs));
    }
    player *pl = gk->map_idToplayer[rs->m_userid];
    switch (rs->n_lResult) {
        case shoupai:
        {
            auto ite = pl->m_CardBox.begin();
            while(ite!=pl->m_CardBox.end())
            {
                if(gk->IscardEuqal(*ite,&rs->m_card))
                {
                    pl->m_CardBox.erase(ite);

                    return;
                }
                ++ite;
            }
        }
            break;
        case wqpai:
        {
            pl->setwq(0);
        }
        break;
        case fjpai:
        {
            pl->setfj(0);
        }
        break;
        case jgmpai:
        {
            pl->setjgm(0);
        }
        break;
        case fympai:
        {
            pl->setfym(0);
        }
        break;
        default:
            break;
    }
}

//????????????
void TcpKernel::ChangeTurn(int clientfd, char *szbuf, int nlen)
{
    STRU_OFFCARD_RQ * rq = (STRU_OFFCARD_RQ *)szbuf;
    GameKernel *gk = m_RoomManger->map_gamekl[rq->m_roomid];
    player *pl = gk->map_idToplayer[rq->m_user_id];
    int i=0;
    while(1)
    {
        if(rq->m_offcard[i].id == 0)
            break;
        //????????????
        auto ite = pl->m_CardBox.begin();
                while(ite!=pl->m_CardBox.end())
                {
                    if(gk->IscardEuqal(*ite,&rq->m_offcard[i]))
                    {
                        pl->m_CardBox.erase(ite);
                        break;
                    }
                    ++ite;
                }
        gk->FUN_OffCard(&rq->m_offcard[i]);
        i++;
    }
    //????????????
    for(int i=0;i<5;i++)
    {
        int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
        if(sockfd == clientfd)
            continue;
        m_tcp->SendData(sockfd,szbuf,nlen);
    }
    gk->nextTurn();
    STRU_TURN_BEGIN tb;
    tb.user_id = gk->idarr[gk->currentTurn];
    for(int i=0;i<5;i++)
    {

        int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
        m_tcp->SendData(sockfd,(char *)&tb,sizeof(tb));
    }

}
//????????????
void TcpKernel::updateHp(int user_id,int hpchange, GameKernel *gamek)
{
    GameKernel *gk = gamek;
    STRU_COMMIT_STATUS scs;
    scs.user_id = user_id;

    scs.hp_change = gk->map_idToplayer[user_id]->changeHp(hpchange);
    for(int i=0;i<5;i++)
    {
        int sockfd = gk->map_sockfd[gk->idarr[i]];
        m_tcp->SendData(sockfd,(char *)&scs,sizeof(scs));
    }
}

void TcpKernel::HealRs(int clientfd, char *szbuf, int nlen)
{
    STRU_SELHERO_RS * rs = (STRU_SELHERO_RS*)szbuf;
    GameKernel *gk = m_RoomManger->map_gamekl[rs->room_id];
    gk->HealRs(szbuf);
}

//????????????
void TcpKernel::LeaveRoom(int clientfd, char *szbuf, int nlen)
{
    printf("sockfd:%d\t????????????\n",clientfd);
    STRU_LEAVEROOM_RQ* rq = (STRU_LEAVEROOM_RQ*)szbuf;
    if(!m_RoomManger->leaveRoom(rq->m_RoomId,rq->m_userId))
    {
        printf("??????%d?????????\n",rq->m_RoomId);
        char szsql[_DEF_SQLIEN] = {0};
        sprintf(szsql,"delete from t_room where room_id = %d;",rq->m_RoomId);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        return ;
    }
    GameKernel *gk = m_RoomManger->map_gamekl[rq->m_RoomId];
    //if(!gk->gamestart)
    UpdateRoomMemberInfo(rq->m_RoomId);
}

//????????????????????????
void TcpKernel::UpdateRoomMemberInfo(int room_id)
{
    GameKernel *gk = m_RoomManger->map_gamekl[room_id];
    STRU_ROOM_MEMBER_RS rs;

    int times = 0;
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i]!=0)
        {
            times = 0;
            for(int j=0;j<5;j++)
            {
                if(gk->idarr[j]!=0&&i!=j)
                {
                    int user_id = gk->idarr[j];
                    rs.m_userInfo[times].m_userid = user_id;
                    rs.m_userInfo[times].m_iconid = map_IdtoUserInfo[user_id]->icon_id;
                    strcpy(rs.m_userInfo[times].m_szName,map_IdtoUserInfo[user_id]->m_szName);
                    strcpy(rs.m_userInfo[times].m_szFelling,map_IdtoUserInfo[user_id]->m_szFelling);
                    rs.m_userInfo[times].status = 1;
                    rs.m_userInfo[times].m_place = j;
                    times++;
                }
            }
            int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
            rs.m_nType = DEF_PACK_ROOM_MEMBER_RS;
            m_tcp->SendData(sockfd,(char *)&rs,sizeof(rs));
            memset((char *)&rs,0,sizeof(rs));
        }
    }
}

//????????????
void TcpKernel::SearchRoom(int clientfd, char *szbuf, int nlen)
{
    printf("SearchRoom\n");

    STRU_SEARCH_ROOM_RQ *rq = (STRU_SEARCH_ROOM_RQ *)szbuf;
    STRU_SEARCH_ROOM_RS rs;
    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    if(rq->m_Roomid!=0)
    {
        //???id????????????
        sprintf(szsql,"select room_id ,room_name,room_creator_name from t_room where room_id = %d;"
                ,rq->m_Roomid);
        if(!m_sql->SelectMysql(szsql,3,ls))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        if(ls.size()==0)
        {
            rs.m_lResult = search_room_failed;
        }
        else
        {
            rs.m_lResult = search_room_success;
            rs.m_roomInfo.m_Roomid = atoi(ls.front().c_str());      ls.pop_front();
            strcpy(rs.m_roomInfo.sz_Roomname,ls.front().c_str());   ls.pop_front();
            strcpy(rs.m_roomInfo.sz_RoomCreator,ls.front().c_str());
            rs.m_roomInfo.m_num = m_RoomManger->map_gamekl[rq->m_Roomid]->num;
        }
    }
    else
    {
        //?????????????????????
        sprintf(szsql,"select room_id ,room_name,room_creator_name from t_room where room_name = '%s';"
                ,rq->m_szRoomName);
        if(!m_sql->SelectMysql(szsql,3,ls))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        if(ls.size()==0)
        {
            rs.m_lResult = search_room_failed;
        }
        else
        {
            rs.m_lResult = search_room_success;
            rs.m_roomInfo.m_Roomid = atoi(ls.front().c_str());      ls.pop_front();
            strcpy(rs.m_roomInfo.sz_Roomname,ls.front().c_str());   ls.pop_front();
            strcpy(rs.m_roomInfo.sz_RoomCreator,ls.front().c_str());
            rs.m_roomInfo.m_num = m_RoomManger->map_gamekl[rq->m_Roomid]->num;

        }
    }
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}

//??????????????????
void TcpKernel::CheckOfflineMsg(int clientfd, char *szbuf, int nlen)
{
    printf("CheckOfflineMsg\n");

    //??????????????????????????????
    STRU_OFFLINE_RQ * rq;
    int user_id = rq->m_userID;
    list<string> ls;
    list<string> name_ls;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"select fri_rq_id from t_addfriend where user_id = %d;",user_id);
    m_sql->SelectMysql(szsql,1,ls);
    for(int i=0;i<ls.size();i++)
    {
        STRU_ADD_FRIEND_RQ add_rq;
        add_rq.m_userInfo.m_userid = atoi(ls.front().c_str());     ls.pop_front();

        add_rq.m_friInfo.m_userid = user_id;
        add_rq.m_friInfo.m_iconid = map_IdtoUserInfo[user_id]->icon_id;
        strcpy(add_rq.m_friInfo.m_szName,map_IdtoUserInfo[user_id]->m_szName);
        strcpy(add_rq.m_friInfo.m_szFelling,map_IdtoUserInfo[user_id]->m_szFelling);
        add_rq.m_friInfo.status = map_IdtoUserInfo[user_id]->icon_id;

        bzero(szsql,sizeof(szsql));
        sprintf(szsql,"select pic_id,user_name,felling,status from t_userInfo where user_id = %d;",add_rq.m_userInfo.m_userid);
        if(!m_sql->SelectMysql(szsql,1,name_ls))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        add_rq.m_userInfo.m_iconid = atoi(ls.front().c_str());      ls.pop_front();
        strcpy(add_rq.m_userInfo.m_szName,ls.front().c_str());      ls.pop_front();
        strcpy(add_rq.m_userInfo.m_szFelling,ls.front().c_str());   ls.pop_front();
        add_rq.m_userInfo.status = atoi(ls.front().c_str());        ls.pop_front();

        AddfriendRq(clientfd,(char *)&add_rq,sizeof(add_rq));
        bzero(szsql,sizeof(szsql));
        sprintf(szsql,"delete from t_addfriend where fri_rq_id = %d;",add_rq.m_userInfo.m_userid);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }

    }
    //????????????????????????
}

//??????????????????
void TcpKernel::AlterUserInfo(int clientfd, char *szbuf, int nlen)
{
    STRU_ALTER_USERINFO_RQ *rq = (STRU_ALTER_USERINFO_RQ*)szbuf;
    STRU_ALTER_USERINFO_RS rs;
    char szsql[_DEF_SQLIEN] = {0};
    list<string> ls;
    snprintf(szsql,sizeof(szsql),"select user_name from t_userInfo where user_name = '%s';",rq->m_userInfo.m_szName);
    m_sql->SelectMysql(szsql,1,ls);
    if(ls.size()==0)
    {
        rs.m_result = alter_success;
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"update t_userInfo set pic_id = %d,user_name = '%s',felling = '%s' where user_id = %d;"
                 ,rq->m_userInfo.m_iconid,rq->m_userInfo.m_szName,rq->m_userInfo.m_szFelling,rq->m_userInfo.m_userid);
        m_sql->UpdataMysql(szsql);
    }
    else
        rs.m_result = name_repeat;

    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rq));

}

//??????
void TcpKernel::OffLine(int clientfd, char *szbuf, int nlen)
{
    printf("OffLine\n");

    STRU_OFFLINE_RQ *rq = (STRU_OFFLINE_RQ *)szbuf;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"update t_userInfo set status = 0  where user_id = %d; ",rq->m_userID);
    m_sql->UpdataMysql(szsql);
}



