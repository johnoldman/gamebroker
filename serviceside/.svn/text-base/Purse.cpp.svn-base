//
// $Id: Purse.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "Purse.h"

#include <stdexcept>
#include <boost/lexical_cast.hpp>

#include "Ledger.h"
#include "UserSessionId.h"
#include "PersistentStore.h"
#include "GameSessionId.h"
#include "NotEnoughMoneyException.h"
#include "Payment.h"

// Purse constructor - when creating a new one

Purse::Purse(PersistentStore& store, const GameSessionId& id, const std::string& key, long userId, long gameCost, long floorLimit, const std::string& tariffid, boost::shared_ptr<ITariff> tariff, Ledger& ledger, const std::string& purseReference)
: m_store(store)
, m_key(key)
, m_userid(userId)
, m_tariff(tariff)
, m_ledger(ledger)
// The game session + user session id should be unique, so use it as a message id for the ledger withdrawal.
, m_assets(ledger.payPurse(userId, tariffid, gameCost + floorLimit, id.toString(), id.toString() + purseReference))
, m_purseid(store.savePurse(id, PurseData(m_key, m_userid, tariffid, purseReference)))
, m_tariffid(tariffid)
, m_gameid(id)
{
   for (std::vector<Asset>::const_iterator it = m_assets.begin(); it != m_assets.end(); ++it)
   {
      it->save(store, m_purseid);
      PaymentTransaction pt(it->getAmount(), it->getId(), time(NULL), "Purse Creation", it->getFromId(), true, id.toString() + purseReference, m_userid, m_userid);
      m_store.savePurseTransaction(m_purseid, pt);
      m_transactions.push_back(pt);
   }
}

// Purse constructor - when loading from backing store

Purse::Purse(PersistentStore& store, const GameSessionId& gamesessionid, const PurseId& purseid, long userId, const std::string& tariffid, boost::shared_ptr<ITariff> tariff, Ledger& ledger)
: m_store(store)
, m_userid(userId)
, m_tariff(tariff)
, m_ledger(ledger)
, m_purseid(purseid)
, m_tariffid(tariffid)
, m_gameid(gamesessionid)
{
   std::vector<AssetData> assets = store.loadAssets(purseid);
   for (std::vector<AssetData>::const_iterator it = assets.begin(); it != assets.end(); ++it)
   {
      m_assets.push_back(Asset(it->getAmount(), it->getId(), it->getFromId()));
   }
   m_transactions = store.loadTransactions(purseid);
}
//
// Called just as the purse is about to go away - send results back to ledger & remove from database
//
void Purse::close(const std::string& messageid)
{
   Payment payment(m_userid, messageid);
   for (std::vector<PaymentTransaction>::const_iterator it = m_transactions.begin(); it != m_transactions.end(); ++it)
   {
      payment.addTransaction(*it);
   }
   m_ledger.payWallet(m_userid, m_tariffid, payment);
   m_store.deletePurse(m_gameid, m_purseid);
}

long Purse::getTotal() const
{
   long total = 0;
   for (std::vector<Asset>::const_iterator it = m_assets.begin(); it != m_assets.end(); ++it)
   {
      total += it->getAmount();
   }
   return total;
}

std::vector<Asset> Purse::removeAssets(Amount amount, const std::string& eventCode, time_t eventTime, long otheruserid, const std::string& reference)
{
   std::vector<Asset> removed;
   WinType w(eventCode);

   // Need to take copy of assets in case exception happens.
   // Persistent store will be handled by database rollback.
   std::vector<Asset> purseAssets(m_assets);
   std::vector<PaymentTransaction> transactions;

   std::vector<Asset>::iterator it = purseAssets.begin();
   while (it != purseAssets.end() && amount > 0)
   {
      AssetClass oldClass(it->getId());
      std::auto_ptr<AssetClass> ac = m_tariff->getAssetMapping(w, oldClass);
      if (ac.get() == 0)
      {
         throw std::runtime_error(std::string("Purse[") + boost::lexical_cast<std::string>(m_purseid) + "]::removeAssets no mapping for wintype " + eventCode + " asset class " + it->getClassName() + " in tariff " + m_tariffid);
      }
      Amount amountRemoved(0);
      if (amount < it->getAmount())
      {
         amountRemoved = amount;
         it->remove(amount);
         it->update(m_store, m_purseid);
         removed.push_back(Asset(amount, ac->getId(), it->getId()));
         amount = Amount(0);
         ++it;
      }
      else
      {
         amountRemoved = it->getAmount();
         it->destroy(m_store, m_purseid);
         amount -= it->getAmount();
         it = purseAssets.erase(it);
      } 
      PaymentTransaction pt(amountRemoved, ac->getId(), eventTime, eventCode, ac->getId(), false, reference, m_userid, otheruserid);
      m_store.savePurseTransaction(m_purseid, pt);
      transactions.push_back(pt);
   }
   if (amount > 0)
   {
      throw NotEnoughMoneyException(std::string("Event Code ") + eventCode + " Purse::removeAssets ", m_purseid);
   }
   // Make sure that assignment to member variable can't throw.
   std::swap(purseAssets, m_assets);

   // Having survived the loop, put the new transactions into the object so they will be remembered.
   //m_transactions.append(transactions);
   for (std::vector<PaymentTransaction>::const_iterator it = transactions.begin(); it != transactions.end(); ++it)
   {
      m_transactions.push_back(*it);
   }

   return removed;
}

void Purse::putAssets(const std::vector<Asset>& assets, const std::string& eventCode, time_t eventTime, long otheruserid, const std::string& reference)
{
   for (std::vector<Asset>::const_iterator it = assets.begin(); it != assets.end(); ++it)
   {
      bool bFound = false;
      for (std::vector<Asset>::iterator ourAssets = m_assets.begin(); ourAssets != m_assets.end(); ++ourAssets)
      {
         if (ourAssets->getClassName() == it->getClassName())
         {
            *ourAssets += *it;
            ourAssets->update(m_store, m_purseid);
            bFound = true;
         }
      }
      if (!bFound)
      {
         m_assets.push_back(*it);
         it->save(m_store, m_purseid);
      }
      PaymentTransaction pt(it->getAmount(), it->getId(), eventTime, eventCode, it->getFromId(), true, reference, m_userid, otheruserid);
      m_store.savePurseTransaction(m_purseid, pt);
      m_transactions.push_back(pt);
   }
}
