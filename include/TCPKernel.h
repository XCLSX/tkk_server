#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H



#include "TCPNet.h"
#include "Mysql.h"
#include <croommanger.h>
#include<cardmanger.h>
class TcpKernel;
typedef void (TcpKernel::*PFUN)(int,char*,int nlen);

typedef struct
{
    PackType m_type;
    PFUN m_pfun;
} ProtocolMap;

typedef struct STRU_USER_INFO_S
{
    STRU_USER_INFO_S()
    {
        sockfd = 0;
        icon_id = 0;
        memset(m_szName,0,MAX_SIZE);
        memset(m_szFelling,0,MAX_SIZE);
        status = 0;
    }
    int sockfd;
    int icon_id;
    char m_szName[MAX_SIZE];
    char m_szFelling[MAX_SIZE];
    int status;
}STRU_USER_INFO_S;


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

    //查找好友
    void SearchFriend(int,char*,int);

    //添加好友请求
    void AddfriendRq(int,char*,int);

    //添加好友回复
    void AddfriendRs(int,char*,int);

    //获取好友列表
    void GetFriList(int,char *,int);

    //创建房间
    void CreateRoom(int,char*,int);

    //查询房间
    void SearchRoom(int,char *,int);

    //刷新房间列表
    void AskRoom(int,char*,int);

    //加入房间
    void JoinRoom(int,char *,int);

    //开始游戏
    void StartGame(int,char*,int);

    //选择武将请求
    void SelHeroRq(int id,int roomid,bool isZG);
    //选择武将回复
    void SelHeroRs(int,char*,int);
    //离开房间
    void LeaveRoom(int,char *,int);

    //更新房间成员数据
    void UpdateRoomMemberInfo(int);
    //查询离线数据
    void CheckOfflineMsg(int,char *,int);

    //修改用户信息
    void AlterUserInfo(int,char*,int);

    //离线
    void OffLine(int ,char *,int);

 private:
    CMysql * m_sql;
    TcpNet * m_tcp;
    map<int,STRU_USER_INFO_S*> map_IdtoUserInfo;
    CRoomManger *m_RoomManger;
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
