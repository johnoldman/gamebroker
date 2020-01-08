//
// $Id: Asset.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "Asset.h"
#include "PersistentStore.h"

Asset::Asset(const Amount& amount, int id, int fromId)
: m_amount(amount)
, m_className("fred")
, m_id(id)
, m_fromId(fromId)
{
}

Asset::Asset(const Amount& amount, int id)
: m_amount(amount)
, m_className("fred")
, m_id(id)
, m_fromId(id)
{
}

void Asset::save(PersistentStore& store, const PurseId& id) const
{
   store.saveAsset(id, m_amount, m_className, m_id);
}

void Asset::update(PersistentStore& store, const PurseId& id) const
{
   store.updateAsset(id, m_amount, m_className, m_id);
}

Amount Asset::getAmount() const
{
   return m_amount;
}

std::string Asset::getClassName() const
{
   return m_className;
}

void Asset::remove(const Amount& amount)
{
   m_amount -= amount;
}

Asset& Asset::operator+=(const Asset& other)
{
   m_amount += other.m_amount;
   return *this;
}

void Asset::destroy(PersistentStore& store, const PurseId& id)
{
//   store.removeAsset(id, m_className);
}

int Asset::getId() const
{
   return m_id;
}

int Asset::getFromId() const
{
   return m_fromId;
}
