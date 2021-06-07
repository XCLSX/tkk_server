#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H



#include "TCPNet.h"
#include "Mysql.h"

class TcpKernel;
typedef void (TcpKernel::*PFUN)(int,char*,int nlen);

typedef struct
{
    PackType m_type;
    PFUN m_pfun;
} ProtocolMap;



class TcpKernel:public IKernel
{
public:
    int Open();
    void Close();
    void DealData(int,char*,int);

    //注册
    void Register(int,char*,int);
    //登录
    void Login(int,char*,int);
    //刷新房间列表
    void AskRoom(int,char*,int);
    //创建房间
    void CreateRoom(int,char*,int);
    //查找好友
    void SearchFriend(int,char*,int);
    //添加好友
    void Addfriend(int,char*,int);
    //接受好友申请
    void PostFriendRq(int,int);
    //接受好友回复
    void PostFriendRs(int,char *,int);
    //查询离线数据
    void CheckOfflineMsg(int,char *,int);
    //离线
    void OffLine(int ,char *,int);

 private:
    CMysql * m_sql;
    TcpNet * m_tcp;
    map<int,int> m_socketmap;
};






////读取上传的视频流信息
//void UpLoadVideoInfoRq(int,char*);
//void UpLoadVideoStreamRq(int,char*);
//void GetVideoRq(int,char*);
//char* GetVideoPath(char*);
//void QuitRq(int,char*);
//void PraiseVideoRq(int,char*);
//void GetAuthorInfoRq(int,char*);
#endif
