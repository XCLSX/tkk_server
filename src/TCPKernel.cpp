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
    {DEF_PACK_ADD_FRIEND_RQ,&TcpKernel::Addfriend},
    {DEF_PACK_SEARCH_FRIEND_RQ,&TcpKernel::SearchFriend},
    {DEF_PACK_DEAL_FRIEND_RS,&TcpKernel::PostFriendRs},
    {DEF_PACK_CHECKOFFLINE_RQ,&TcpKernel::CheckOfflineMsg},
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
    m_sql->SelectMysql(szsql,2,ls);
    //用户名或邮箱已被使用
    if(ls.size()>0)
    {
        rs.m_lResult = userid_is_exist;
    }
    else
    {   //插入账户密码邮箱-> t_user
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"insert into t_user values (null,'%s','%s','%s');",rq->m_szUser,rq->m_szEmall,rq->m_szPassword);
        m_sql->UpdataMysql(szsql);
        //默认值初始化个人信息
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"insert into t_userInfo values(null,1,'未命名_%s',' 这个人很懒，没什么没什么想说的。',0,0);",rq->m_szUser);
        m_sql->UpdataMysql(szsql);


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
    snprintf(szsql,sizeof(szsql),"select user_pwd from t_user where user_account = '%s';"
             ,rq->m_szUser);
    m_sql->SelectMysql(szsql,1,ls);
    if(ls.size()==0)
    {
        rs.m_lResult =userid_no_exist;
    }
    else
    {
        if(strcmp(ls.front().c_str(),rq->m_szPassword)==0)
        {
            //登录成功
            rs.m_lResult = login_sucess;

            //获取u_id
            int user_id;
            bzero(szsql,sizeof(szsql));
            ls.clear();
            snprintf(szsql,sizeof(szsql),"select user_id from t_user where user_account='%s';",rq->m_szUser);
            m_sql->SelectMysql(szsql,1,ls);
            user_id = atoi(ls.front().c_str());
            m_socketmap[user_id] = clientfd;
            //用户登录状态改为1 更新sockfd到数据库
            bzero(szsql,sizeof(szsql));
            snprintf(szsql,sizeof(szsql),"update t_userInfo set status = 1  where user_id = %d;"
                                         ,user_id);
            m_sql->UpdataMysql(szsql);


            rs.m_userid = user_id;
            //获取用户基本信息
            bzero(szsql,sizeof(szsql));
            snprintf(szsql,sizeof(szsql),"select * from t_userInfo where user_id = %d;",user_id);
            ls.clear();
            m_sql->SelectMysql(szsql,4,ls);
            //初始化信息
            ls.pop_front();
            rs.m_userInfo.m_iconID = atoi(ls.front().c_str());  ls.pop_front();
            strcpy(rs.m_userInfo.m_szName,ls.front().c_str());  ls.pop_front();
            strcpy(rs.m_userInfo.m_feeling,ls.front().c_str());  ls.pop_front();
            rs.m_userInfo.m_state = 1;

        }
        else
            rs.m_lResult = password_error;
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
    m_sql->SelectMysql(szsql,3,ls);
    int i=0;
    if(ls.size()>0)
        rs.m_lResult = ask_room_success;
    else
        rs.m_lResult = ask_room_failed;
    while(ls.size()>0)
    {
        rs.m_RoomList[i].m_Roomid = atoi(ls.front().c_str()); ls.pop_front();
        strcpy(rs.m_RoomList[i].sz_Roomname,ls.front().c_str());  ls.pop_front();
        strcpy(rs.m_RoomList[i].sz_RoomCreator,ls.front().c_str());   ls.pop_front();
        i++;
        if(i==MAX_ROOMLIST)
            break;
    }
    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));

}
//创建房间
void TcpKernel::CreateRoom(int clientfd, char *szbuf, int nlen)
{
    printf("CreateRoom\n");

    STRU_CREATEROOM_RQ *rq = (STRU_CREATEROOM_RQ*)szbuf;
    STRU_CREATEROOM_RS rs;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"insert into t_room values(null,'%s',%d,null,null,null,null,'%s');"
             ,rq->m_RoomName,rq->m_userid,rq->m_RoomName);
    m_sql->UpdataMysql(szsql);
    list<string> ls;
    bzero(szsql,sizeof(szsql));
    snprintf(szsql,sizeof(szsql),"select room_id from t_room where user1_id = %d;",rq->m_userid);
    m_sql->SelectMysql(szsql,1,ls);
    if(ls.size()==0)
        rs.m_lResult = create_failed;
    else
    {
        rs.m_lResult = create_success;
        rs.m_RoomId = atoi(ls.front().c_str());
    }
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
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
    m_sql->SelectMysql(szsql,5,ls);
    if(ls.size()==0)
    {
        //此用户不存在
        rs.m_result = no_this_user;
        m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
        return ;

    }
    rs.m_friend_info.m_friend = atoi(ls.front().c_str());   ls.pop_front();
    rs.m_friend_info.m_iconID = atoi(ls.front().c_str());   ls.pop_front();
    strcpy(rs.m_friend_info.m_szName,ls.front().c_str());   ls.pop_front();
    strcpy(rs.m_friend_info.m_feeling,ls.front().c_str());  ls.pop_front();
    rs.m_friend_info.m_state = atoi(ls.front().c_str());    ls.clear();

    //判断是否已为好友
    bzero(szsql,sizeof(szsql));
    snprintf(szsql,sizeof(szsql),"select user_id from t_friend where user_id = %d and friend_id = %d;"
             ,rq->m_userid,rs.m_friend_info.m_friend);
    m_sql->SelectMysql(szsql,1,ls);
    if(ls.size()==0)
        rs.m_result = search_success;
    else
        rs.m_result = is_your_friend;
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );

}
//添加好友
void TcpKernel::Addfriend(int clientfd, char *szbuf, int nlen)
{
    printf("Addfriend\n");

    STRU_ADD_FRIEND_RQ *rq = (STRU_ADD_FRIEND_RQ*)szbuf;
    STRU_ADD_FRIEND_RS rs;
    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"select status, from t_userInfo where user_name = '%s';"
             ,rq->m_szAddFriendName);
    m_sql->SelectMysql(szsql,1,ls);
    if(ls.front().c_str()==0)
    {
        //用户离线
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"insert into t_addfriend values(%d,%d);",rq->m_friendID,rq->m_userID);
        rs.m_nType = add_wait;
    }
    else
    {
        //用户在线 发送好友请求
        PostFriendRq(m_socketmap[rq->m_friendID],rq->m_userID);

    //    m_tcp->SendData(friend_fd, (char *),);
    }
    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}
//接受好友请求
void TcpKernel::PostFriendRq(int clientfd, int user_id)
{
    printf("PostFriendRq\n");
    STRU_DEAL_FRIEND_RQ rq;
    char szsql[_DEF_SQLIEN] = {0};
    list<string> ls;
    snprintf(szsql,sizeof(szsql),"select user_id,pic_id,user_name,felling,status from t_userInfo where user_id = %d;",user_id);
    m_sql->SelectMysql(szsql,5,ls);
    rq.m_friend_info.m_friend = atoi(ls.front().c_str());   ls.pop_front();
    rq.m_friend_info.m_iconID = atoi(ls.front().c_str());   ls.pop_front();
    strcpy(rq.m_friend_info.m_szName,ls.front().c_str());   ls.pop_front();
    strcpy(rq.m_friend_info.m_feeling,ls.front().c_str());  ls.pop_front();
    rq.m_friend_info.m_state = atoi(ls.front().c_str());
    m_tcp->SendData(clientfd,(char *)&rq,sizeof(rq));


}

//接受好友回复
void TcpKernel::PostFriendRs(int clientfd, char *szbuf, int nlen)
{
    printf("PostFriendRs\n");

    STRU_DEAL_FRIEND_RS* rs =(STRU_DEAL_FRIEND_RS*)szbuf;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"delete from t_addfriend where user_id = %d and fri_rq_id = %d;"
             ,rs->m_userid,rs->m_friendid);
    m_sql->UpdataMysql(szsql);
    if(rs->m_result == add_success)
    {
        //添加成功
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"insert into values(%d,%d);",rs->m_userid,rs->m_friendid);
        m_sql->UpdataMysql(szsql);
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"insert into values(%d,%d);",rs->m_friendid,rs->m_userid);
        m_sql->UpdataMysql(szsql);
    }
}

void TcpKernel::CheckOfflineMsg(int clientfd, char *szbuf, int nlen)
{
    printf("CheckOfflineMsg\n");

    //查询离线添加好友数据
    STRU_CHECK_OFFLINEMSG_RQ *rq = (STRU_CHECK_OFFLINEMSG_RQ*)szbuf;
    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"select fri_rq_id from t_addfriend where user_id = %d;",rq->m_userid);
    m_sql->SelectMysql(szsql,1,ls);
    for(int i=0;i<ls.size();i++)
    {
        PostFriendRq(clientfd,atoi(ls.front().c_str()));
        ls.pop_front();
    }
    //查询离线消息数据
}

void TcpKernel::AlterUserInfo(int clientfd, char *szbuf, int nlen)
{
    STRU_ALTER_USERINFO_RQ *rq = (STRU_ALTER_USERINFO_RQ*)szbuf;
    STRU_ALTER_USERINFO_RS rs;
    char szsql[_DEF_SQLIEN] = {0};
    list<string> ls;
    snprintf(szsql,sizeof(szsql),"select user_name from t_userInfo where user_name = '%s';",rq->sz_userName);
    m_sql->SelectMysql(szsql,1,ls);
    if(ls.size()==0)
    {
        rs.m_result = alter_success;
        bzero(szsql,sizeof(szsql));
        snprintf(szsql,sizeof(szsql),"update t_userInfo set pic_id = %d,user_name = '%s',felling = '%s' where user_id = %d;"
                 ,rq->m_iconid,rq->sz_userName,rq->sz_felling,rq->user_id);
        m_sql->UpdataMysql(szsql);
    }
    else
        rs.m_result = name_repeat;

    m_tcp->SendData(clientfd,(char *)&rq,sizeof(rq));

}
//离线
void TcpKernel::OffLine(int clientfd, char *szbuf, int nlen)
{
    printf("OffLine\n");

    STRU_OFFLINE_RQ *rq = (STRU_OFFLINE_RQ *)szbuf;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"update t_userInfo set status = 0,sock_fd = -1  where user_id = %d; ",rq->m_userID);
    m_sql->UpdataMysql(szsql);
}



