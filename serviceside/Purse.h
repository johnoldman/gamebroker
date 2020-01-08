//
// $Id: Purse.h 3484 2007-12-12 11:57:57Z stephend $
//
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>

#include "Asset.h"
#include "ITariff.h"
#include "PaymentTransaction.h"
#include "GameSessionId.h"
#include "PurseId.h"

class Ledger;
class UserSessionId;
class PersistentStore;

class Purse : public boost::noncopyable
{
public:
   Purse(PersistentStore& store, const GameSessionId& gsi, const std::string& key, long userId, long gameCost, long floorLimit, const std::string& tariffid, boost::shared_ptr<ITariff> tariff, Ledger& ledger, const std::string& purseReference);
   Purse(PersistentStore& store, const GameSessionId& gsi, const PurseId& id, long userId, const std::string& tariffid, boost::shared_ptr<ITariff> tariff, Ledger& ledger);
   std::vector<Asset> removeAssets(Amount amount, const std::string& eventCode, time_t, long, const std::string&);
   void putAssets(const std::vector<Asset>& assets, const std::string& eventCode, time_t, long, const std::string&);
   long getTotal() const;
   void close(const std::string& messageid);
   PurseId getId() const { return m_purseid; }
   long getUserId() const { return m_userid; }
private:
   PersistentStore& m_store;
   const std::string m_key;
   const long m_userid;
   boost::shared_ptr<ITariff> m_tariff;
   Ledger& m_ledger;
   std::vector<Asset> m_assets;
   const PurseId m_purseid;
   const std::string m_tariffid;
   std::vector<PaymentTransaction> m_transactions;
   const GameSessionId m_gameid;
};
