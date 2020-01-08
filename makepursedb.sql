--
-- Table structure for table `TransactionLog`
--
DROP TABLE IF EXISTS TransactionLog;
CREATE TABLE TransactionLog (
  messageid varchar(100) not NULL,
  message varchar(300) not NULL,
  response varchar(400) not NULL,
  primary key(messageid)
);

drop table if exists PurseTransactions;
drop table if exists PurseAssets;
drop table if exists PurseStore;
drop table if exists GameInstanceStore;

create table GameInstanceStore (
   sessionid varchar(100) not NULL,
   gameName varchar(100) not null,
   gameType varchar(100) not null,
   gameTariff varchar(100) not null,
   primary key(sessionid)
);

create table PurseStore (
   purseid int not null auto_increment,
   gamesessionid varchar(100) not null,
   storagekey varchar(250) not null,
   userid long not null,
   tariffname varchar(20) not null,
   uniquereference varchar(250) not null,
   primary key(purseid),
   index(gamesessionid),
   foreign key(gamesessionid) references GameInstanceStore(sessionid)
);

create table PurseAssets (
   purseid int not null,
   amount int not null,
   assetclass varchar(20) not null,
   assetid int not null,
   primary key(purseid, assetid),
   index(purseid),
   foreign key(purseid) references PurseStore(purseid) on delete cascade
);

create table PurseTransactions (
   purseid int not null,
   id int not null auto_increment,
   amount int not null,
   assetid int not null,
   timestamp long not null,
   description varchar(100) not null,
   fromassetid int not null,
   iscredit tinyint not null,
   reference varchar(250) not null,
   userid long not null,
   otheruserid long not null,
   primary key(id),
   foreign key(purseid) references PurseStore(purseid) on delete cascade
);
