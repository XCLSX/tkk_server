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





//注册
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
    //用户名或邮箱已被使用
    if(ls.size()>0)
    {
        rs.m_lResult = userid_is_exist;
    }
    else
    {   //插入账户密码邮箱-> t_user
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"insert into t_user values (null,'%s','%s','%s');",rq->m_szUser,rq->m_szEmall,rq->m_szPassword);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        //默认值初始化个人信息
        bzero(szsql,sizeof(szsql));
        sprintf(szsql,"select user_id from t_user where user_account = '%s';",rq->m_szUser);
        if(!m_sql->SelectMysql(szsql,1,ls))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        snprintf(szsql,sizeof(szsql),"insert into t_userInfo values(%d,1,'%s',' 这个人很懒，没什么没什么想说的。',0);"
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

//登录
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
        //登录成功
        rs.m_lResult = login_sucess;
        //查询用户基本信息 并将登录状态置为1
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
        //缓存在线用户信息到本地
        STRU_USER_INFO_S *userInfo = new STRU_USER_INFO_S;
        userInfo->sockfd = clientfd;
        userInfo->status = 1;
        userInfo->icon_id = atoi(ls.front().c_str());             ls.pop_front();
        strcpy(userInfo->m_szName,ls.front().c_str());            ls.pop_front();
        strcpy(userInfo->m_szFelling,ls.front().c_str());
        map_IdtoUserInfo[user_id] = userInfo;
        //完善包
        rs.m_userInfo.m_userid = user_id;
        rs.m_userInfo.status = 1;
        rs.m_userInfo.m_iconid = userInfo->icon_id;
        strcpy(rs.m_userInfo.m_szName,userInfo->m_szName);
        strcpy(rs.m_userInfo.m_szFelling,userInfo->m_szFelling);
    }
    else
           rs.m_lResult = password_error;

    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
    //获取离线信息
}

//查找好友
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
        //此用户不存在
        rs.m_result = no_this_user;
        m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
        return ;

    }
    rs.m_friInfo.m_userid = atoi(ls.front().c_str());   ls.pop_front();
    rs.m_friInfo.m_iconid = atoi(ls.front().c_str());   ls.pop_front();
    strcpy(rs.m_friInfo.m_szName,ls.front().c_str());   ls.pop_front();
    strcpy(rs.m_friInfo.m_szFelling,ls.front().c_str());  ls.pop_front();
    rs.m_friInfo.status = atoi(ls.front().c_str());    ls.clear();

    //判断是否已为好友
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

//添加好友请求
void TcpKernel::AddfriendRq(int clientfd, char *szbuf, int nlen)
{
    printf("Addfriend\n");

    STRU_ADD_FRIEND_RQ *rq = (STRU_ADD_FRIEND_RQ*)szbuf;
    //用户在线
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

//添加好友回复
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

//获取好友列表
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

//创建房间
void TcpKernel::CreateRoom(int clientfd, char *szbuf, int nlen)
{
    printf("CreateRoom\n");
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
        m_RoomManger->CreateRoom(rs.m_RoomId,rq->m_userid);

    }
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}

//刷新房间
void TcpKernel::AskRoom(int clientfd ,char* szbuf,int nlen)
{
    printf("AskRoom\n");

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
        rs.m_RoomList[i].m_num = m_RoomManger->map_uInr[room_id]->num;
        i++;
        if(i==MAX_ROOMLIST)
            break;
    }
    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));

}

//加入房间
void TcpKernel::JoinRoom(int clientfd, char *szbuf, int nlen)
{
    STRU_JOINROOM_RQ *rq = (STRU_JOINROOM_RQ*)szbuf;
    STRU_JOINROOM_RS rs;

    int relt = m_RoomManger->joinRoom(rq->m_RoomID,rq->m_userInfo.m_userid,&rs.place);
    if(relt == 0)
        rs.m_lResult = room_no_exist;
    else if(relt == 1)
        rs.m_lResult = room_is_full;
    rs.m_lResult = join_success;
    int j=0;
    GameKernel *gk = m_RoomManger->map_uInr[rq->m_RoomID];
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i]!=rq->m_userInfo.m_userid&&gk->idarr[i]!=0)
        {
            int id = gk->idarr[i];
            rs.m_userInfoarr[j].m_userid = gk->idarr[i];
            rs.m_userInfoarr[j].m_iconid = map_IdtoUserInfo[id]->icon_id;
            strcpy(rs.m_userInfoarr[j].m_szName,map_IdtoUserInfo[id]->m_szName);
            rs.m_userInfoarr[j].status = 1;
            strcpy(rs.m_userInfoarr[j].m_szFelling,map_IdtoUserInfo[id]->m_szFelling);
        }
    }
    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));

    UpdateRoomMemberInfo(rq->m_RoomID);

}

//开始游戏
void TcpKernel::StartGame(int clientfd, char *szbuf, int nlen)
{
    STRU_STARTGAME_RQ *rq = (STRU_STARTGAME_RQ *)szbuf;
    GameKernel *gk = m_RoomManger->map_uInr[rq->Room_id];
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
    //m_game->m_map[rq->Room_id] = ;
    for(int i=0;i<5;i++)
    {
        int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
        m_tcp->SendData(sockfd,(char *)&rs,sizeof(rs));
    }

    //发放身份
    int arr[5] = {0};
    m_RoomManger->map_CardManger[rq->Room_id]->Freshidentity(arr,5);
    //m_game->Selidentity(arr,5);
    STRU_POST_IDENTITY spi;
    int ZGindex = 0;
    for(int i=0;i<5;i++)
        if(arr[i] == zhugong)
        {
            ZGindex = i;
            break;
        }
    for(int i=0;i<5;i++)
    {
        int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
        spi.m_identity = arr[i];
        spi.m_ZG_userid = gk->idarr[ZGindex];
        m_tcp->SendData(sockfd,(char*)&spi,sizeof(spi));
    }
   //主公选英雄
    SelHeroRq(gk->idarr[ZGindex],rq->Room_id,true);
}

void TcpKernel::SelHeroRq(int id,int roomid,bool isZG)
{
    STRU_SELHERO_RQ rq;
    m_RoomManger->map_CardManger[roomid]->FreshHeroArr(rq.m_HeroArr,0,isZG);
    int sockfd = map_IdtoUserInfo[id]->sockfd;

    m_tcp->SendData(sockfd,(char *)&rq,sizeof(rq));
}

void TcpKernel::SelHeroRs(int clientfd, char *szbuf, int nlen)
{
    STRU_SELHERO_RS *rs = (STRU_SELHERO_RS *)szbuf;
    GameKernel *gk = m_RoomManger->map_uInr[rs->room_id];
    if(rs->isZG)
    {

        gk->InitPlayer(rs->user_id,rs->iddentity,rs->hero_id);
        STRU_SELHERO_RQ rq;

        int arr[16] = {0};
        m_RoomManger->map_CardManger[rs->room_id]->FreshHeroArr(arr,rs->hero_id,false);
        int pos = 0;
        for(int i=0;i<5;i++)
        {
            int k=0;
            if(gk->idarr[i] == rs->user_id)
                continue;
            for(int j = 0;j<4;j++)
            {
                rq.m_HeroArr[k] = arr[pos++];
                k++;
            }
            int sockfd = map_IdtoUserInfo[gk->idarr[i]]->sockfd;
            rq.ZG_heroid = rs->hero_id;
            m_tcp->SendData(sockfd,(char *)&rq,sizeof(rq));
        }
    }
    else
    {
        gk->InitPlayer(rs->user_id,rs->iddentity,rs->hero_id);
        if(gk->map_idToplayer.size()==5)
        {
            //同步英雄信息
            AllSelHero(rs->room_id);
            //发牌


        }
    }
}

void TcpKernel::AllSelHero(int roomid)
{
    GameKernel *gk = m_RoomManger->map_uInr[roomid];
       STRU_ALLSEL_HERO_RS rs;
       for(int j = 0;j<5;j++)
       {
            rs.user_idarr[j] = gk->idarr[j];
            rs.heroarr[j] = gk->map_idToplayer[rs.user_idarr[j]]->m_hero_id;
       }
       for(int i=0;i<5;i++)
       {
           int uid = gk->idarr[i];

           m_tcp->SendData(map_IdtoUserInfo[uid]->sockfd,(char *)&rs,sizeof(rs));

       }
}

//离开房间
void TcpKernel::LeaveRoom(int clientfd, char *szbuf, int nlen)
{
    STRU_LEAVEROOM_RQ* rq = (STRU_LEAVEROOM_RQ*)szbuf;
    if(!m_RoomManger->leaveRoom(rq->m_RoomId,rq->m_userId))
    {
        char szsql[_DEF_SQLIEN] = {0};
        sprintf(szsql,"delete from t_room where room_id = %d;",rq->m_RoomId);
        if(!m_sql->UpdataMysql(szsql))
        {
            printf("sql error:%s\n",szsql);
            return ;
        }
        return ;
    }
    UpdateRoomMemberInfo(rq->m_RoomId);

}

//更新房间成员信息
void TcpKernel::UpdateRoomMemberInfo(int room_id)
{
    GameKernel *gk = m_RoomManger->map_uInr[room_id];
    STRU_ROOM_MEMBER_RS rs;

    int times = 0;
    for(int i=0;i<5;i++)
    {
        if(gk->idarr[i]!=0)
        {
            times = 0;
            for(int j=0;j<5;j++)
            {
                if(gk->idarr[j]!=0&&gk->idarr[j]!=gk->idarr[i])
                {
                    int user_id = gk->idarr[j];
                    rs.m_userInfo[times].m_userid = user_id;
                    rs.m_userInfo[times].m_iconid = map_IdtoUserInfo[user_id]->icon_id;
                    strcpy(rs.m_userInfo[times].m_szName,map_IdtoUserInfo[user_id]->m_szName);
                    strcpy(rs.m_userInfo[times].m_szFelling,map_IdtoUserInfo[user_id]->m_szFelling);
                    rs.m_userInfo[times].status = 1;
                    times++;
                }
            }
            m_tcp->SendData(map_IdtoUserInfo[gk->idarr[i]]->sockfd,(char *)&rs,sizeof(rs));
        }
    }
}

//查找房间
void TcpKernel::SearchRoom(int clientfd, char *szbuf, int nlen)
{
    printf("SearchRoom\n");

    STRU_SEARCH_ROOM_RQ *rq = (STRU_SEARCH_ROOM_RQ *)szbuf;
    STRU_SEARCH_ROOM_RS rs;
    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    if(rq->m_Roomid!=0)
    {
        //按id查询房间
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
            rs.m_roomInfo.m_num = m_RoomManger->map_uInr[rq->m_Roomid]->num;
        }
    }
    else
    {
        //按名称查询房间
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
            rs.m_roomInfo.m_num = m_RoomManger->map_uInr[rq->m_Roomid]->num;

        }
    }
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}

//查询离线信息
void TcpKernel::CheckOfflineMsg(int clientfd, char *szbuf, int nlen)
{
    printf("CheckOfflineMsg\n");

    //查询离线添加好友数据
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
    //查询离线消息数据
}

//修改用户信息
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

//离线
void TcpKernel::OffLine(int clientfd, char *szbuf, int nlen)
{
    printf("OffLine\n");

    STRU_OFFLINE_RQ *rq = (STRU_OFFLINE_RQ *)szbuf;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"update t_userInfo set status = 0  where user_id = %d; ",rq->m_userID);
    m_sql->UpdataMysql(szsql);
}



