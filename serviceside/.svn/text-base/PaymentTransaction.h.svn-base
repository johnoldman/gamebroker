//
// $Id: PaymentTransaction.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef PAYMENT_TRANSACTION_H_
#define PAYMENT_TRANSACTION_H_

#include <ctime>
#include <string>

class PersistentStore;

class PaymentTransaction
{
public:
   PaymentTransaction(long amount, long assetId, time_t eventTime, const std::string& desc, int fromAsset, bool isCredit, const std::string& reference, long userId, long otherUserId);
   long getAmount() const { return m_amount; }
   long getAssetId() const { return assetId_; }
   time_t getDate() const { return eventTime_; }
   std::string getDescription() const { return desc_; }
   long getFromAsset() const { return fromAsset_; }
   bool isCredit() const { return isCredit_; }
   // Game Session Id + User SessionId
   std::string getReference() const { return reference_; }
   long getUserId() const { return userid_; }
   // Other user involved
   long getThirdPartyId() const { return otheruserid_; }
private:
   long m_amount;
   long assetId_;
   time_t eventTime_;
   std::string desc_;
   int fromAsset_;
   bool isCredit_;
   std::string reference_;
   long userid_;
   long otheruserid_;
};

#endif
