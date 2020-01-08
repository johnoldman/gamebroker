//
// $Id: StubLedger.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "StubLedger.h"

void StubLedger::addToPurse(long userId, const Amount& amount, int assetId)
{
   Asset asset(amount, assetId);
   m_purseAmounts.insert(std::make_pair(userId, asset));
}

std::vector<Asset> StubLedger::payPurse(long userId, const std::string& tarrifId, long amount, const std::string& lockerId, const std::string& messageId)
{
   std::map<long, Asset>::const_iterator it = m_purseAmounts.find(userId);
   if (it != m_purseAmounts.end())
   {
      std::vector<Asset> assets;
      assets.push_back(it->second);
      return assets;
   }
   else
   {
      return std::vector<Asset>();
   }
}

long StubLedger::payWallet(long userId, const std::string& tariffId, const Payment& payment)
{
   return 0;
}
