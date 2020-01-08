//
// $Id: Asset.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef ASSET_H_
#define ASSET_H_

#include <boost/shared_ptr.hpp>
#include "AssetClass.h"
#include "Amount.h"

class PersistentStore;
class UserSessionId;
class PurseId;

class Asset
{
public:
   Asset(const Amount& amount, int id, int fromAssetId);
   Asset(const Amount& amount, int id);
   void save(PersistentStore& store, const PurseId& id) const;
   void update(PersistentStore& store, const PurseId& id) const;
   Amount getAmount() const;
   std::string getClassName() const;
   void remove(const Amount& amount);
   void destroy(PersistentStore& store, const PurseId& id);
   Asset& operator+=(const Asset& other);
   int getId() const;
   int getFromId() const;
private:
   Amount m_amount;
   //boost::shared_ptr<AssetClass> m_assetClass;
   std::string m_className;
   int m_id;
   int m_fromId;
};

#endif
