//
// $Id: TransactionLog.h 3484 2007-12-12 11:57:57Z stephend $
//
// class to record every message processed so that in the event of failover or other
// unusual event the result of the message can be replayed.
//
// Given the message/response pairings it probably makes sense to just keep the
// raw message data rather than any more structure which would just need to be
// ressurected later. Probably implements the producer interface to help with this.
// 
//
#ifndef _TRANSACTION_LOG_H_
#define _TRANSACTION_LOG_H_

#include <string>
#include <boost/scoped_ptr.hpp>
#include "MySqlPurseDb.h"

class TransactionLog
{
public:
   TransactionLog();
   ~TransactionLog();
   void storeMessage(const std::string& messageId, const std::string& message);
private:
   const std::string& host;
   const std::string& dbname;
   const std::string& username;
   const std::string& password;
   unsigned short port;
   boost::scoped_ptr<MySqlPurseDb> pStore; 
};

#endif

