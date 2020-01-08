//
// $Id: Payment.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef PAYMENT_H_
#define PAYMENT_H_

#include <string>
#include <vector>

#include "PaymentTransaction.h"

class PaymentVisitor
{
public:
   virtual ~PaymentVisitor() {}
   virtual void visitAmount(long amount) = 0;
};

class Payment
{
public:
   Payment(int userid, const std::string& messageId);
   long getAmount() const { return m_amount; }
   int getUserId() const { return userId_; }
   int getTransactionCount() const { return m_transactions.size(); }
   PaymentTransaction getTransaction(int index) const {
      return m_transactions[index];
   }
   void addTransaction(const PaymentTransaction& txn);

/*   AssetTransactionLog[] getAssetTransactionLog();   // commented out in java code
   void setAssetTransactionLog(AssetTransactionLog[] param); */

   time_t getDate() const { return date_; }
   // GameUserSession + UserSessionId
   std::string getLockerId() const { return lockerId_; }
   void setLockerId(const std::string& param);
   // ????
   std::string getMessageId() const { return messageId_; }
   void setUserId(long param);
   // Needed.
   int getType() const { return 2; }
   //void setType(int param);
   int getState() const { return 4; }
   //void setState(int param);
   long getBalance() const { return m_amount; }
private:
   long m_amount;
   int userId_;   
   std::string messageId_;
   std::string lockerId_;
   int state_;
   int type_;
   std::vector<PaymentTransaction> m_transactions;
   time_t date_;
};

#endif
