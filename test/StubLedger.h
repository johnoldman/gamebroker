//
// $Id: StubLedger.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef STUB_LEDGER_H
#define STUB_LEDGER_H

#include <map>
#include "Ledger.h"

class Payment;

class StubLedger : public Ledger
{
public:
   void addToPurse(long userId, const Amount& amount, int assetid);
public:
   virtual std::vector<Asset> payPurse(long userId, const std::string& tarrifId, long amount, const std::string& lockerId, const std::string& messageId);
   virtual long payWallet(long userId, const std::string& tariffId, const Payment& payment);
private:
   std::map<long, Asset> m_purseAmounts;
};

#endif

