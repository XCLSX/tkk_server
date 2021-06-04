#include<TCPKernel.h>
#include "packdef.h"
#include<stdio.h>

using namespace std;



static const ProtocolMap m_ProtocolMapEntries[] =
{
    {DEF_PACK_REGISTER_RQ , &TcpKernel::RegisterRq},
    {DEF_PACK_LOGIN_RQ , &TcpKernel::LoginRq},
    {0,0}
};
#define RootPath   "/home/colin/Video/"

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
void TcpKernel::RegisterRq(int clientfd,char* szbuf,int nlen)
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
        snprintf(szsql,sizeof(szsql),"insert into t_userInfo values(null,1,'未命名',' 这个人很懒，没什么没什么想说的。',0);");
        m_sql->UpdataMysql(szsql);


        rs.m_lResult = register_sucess;
    }

    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}
//登录
void TcpKernel::LoginRq(int clientfd ,char* szbuf,int nlen)
{
    printf("clientfd:%d LoginRq\n", clientfd);

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
            rs.m_lResult = login_sucess;
            //获取自增的u_id
            bzero(szsql,sizeof(szsql));
            ls.clear();
            snprintf(szsql,sizeof(szsql),"select user_id from t_user where user_account='%s';",rq->m_szUser);
            m_sql->SelectMysql(szsql,1,ls);
            rs.m_userid =atoi(ls.front().c_str());
            //获取用户基本信息
            bzero(szsql,sizeof(szsql));
            snprintf(szsql,sizeof(szsql),"select * from t_userInfo where user_id = '%s';",ls.front().c_str());
            ls.clear();
            m_sql->SelectMysql(szsql,5,ls);
            //初始化信息
            ls.pop_front();
            rs.m_userInfo.m_iconID = atoi(ls.front().c_str());  ls.pop_front();
            strcpy(rs.m_userInfo.m_szName,ls.front().c_str());  ls.pop_front();
            strcpy(rs.m_userInfo.m_feeling,ls.front().c_str());  ls.pop_front();
            rs.m_userInfo.m_state = atoi(ls.front().c_str());

        }
        else
            rs.m_lResult = password_error;
    }

    m_tcp->SendData( clientfd , (char*)&rs , sizeof(rs) );
}

void TcpKernel::AskRoomRq(int clientfd ,char* szbuf,int nlen)
{
    STRU_ASKROOM_RQ *rq = (STRU_ASKROOM_RQ*) szbuf;
    STRU_ASKROOM_RS rs;
    list<string> ls;
    char szsql[_DEF_SQLIEN] = {0};
    snprintf(szsql,sizeof(szsql),"select room_id,room_name,room_creator_name from t_room;");
    m_sql->SelectMysql(szsql,3,ls);
    int i=0;
    while(ls.size()>0)
    {
        rs.m_RoomList[i].m_Roomid = atoi(ls.front().c_str()); ls.pop_front();
        strcmp(rs.m_RoomList[i].sz_Roomname,ls.front().c_str());  ls.pop_front();
        strcmp(rs.m_RoomList[i].sz_RoomCreator,ls.front().c_str());   ls.pop_front();
        i++;
        if(i==MAX_ROOMLIST)
            break;
    }
    m_tcp->SendData(clientfd,(char *)&rs,sizeof(rs));

}



