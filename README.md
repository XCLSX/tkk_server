# tkk_server

#create database tkk;
#use tkk

#create table t_user(user_id int unsigned auto_increment,user_emall varchar(100) not null,user_pwd varchar(40) not null,primary key(user_id));

#create table t_userInfo (user_id int unsigned auto_increment,pic_id int unsigned not null,user_name varchar(10) not null,felling varchar(100),status int unsigned not null,primary key(user_id));

#alter table t_user add column user_account varchar(20) not null after user_id;

#create table t_room (room_id int unsigned auto_increment,room_name varchar(20) not null,,user4_id int unsigned,user5_id int unsigned,primary key(room_id));

#alter table t_room add column room_creator_name varchar(20) not null;

#alter table t_userInfo add unique(user_name);

#create table t_friend(user_id int unsigned ,friend_id int unsigned,primary key(user_id,friend_id));

#create table t_addfriend(user_id int unsigned , fri_rq_id int unsigned ,primary key(user_id,fri_rq_id));

#添加sockfd列
# alter table t_userInfo add column sock_fd int ;
 alter table t_userInfo drop column sock_fd;

#创建视图
create view v_friend as select t_friend.*,t_userInfo.pic_id,t_userInfo.user_name,t_userInfo.felling,t_userInfo.status from t_userInfo inner join t_friend on t_friend.friend_id = t_userInfo.user_id;

