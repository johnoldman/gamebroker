//
// $Id: Ledger.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef LEDGER_H_
#define LEDGER_H_

#include <vector>
#include "Asset.h"

class Payment;

class Ledger
{
public:
   virtual ~Ledger() {}
   virtual std::vector<Asset> payPurse(long userId, const std::string& tarrifId, long amount, const std::string& lockerId, const std::string& messageId) = 0;
   virtual long payWallet(long userId, const std::string& tariffId, const Payment& payment) = 0;
};

#endif
