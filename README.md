# tkk_server

#create database tkk;
#use tkk

#create table t_user(user_id int unsigned auto_increment,user_emall varchar(100) not null,user_pwd varchar(40) not null,primary key(user_id));

#create table t_userInfo (user_id int unsigned auto_increment,pic_id int unsigned not null,user_name varchar(10) not null,felling varchar(100),status int unsigned not null,primary key(user_id));

#alter table t_user add column user_account varchar(20) not null after user_id;

#create table t_room (room_id int unsigned auto_increment,room_name varchar(20) not null,,user4_id int unsigned,user5_id int unsigned,primary key(room_id));

#alter table t_room add column room_creator_name varchar(20) not null;

#alter table t_userInfo add unique(user_name);
