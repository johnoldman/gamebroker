//
// $Id: AssetData.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "AssetData.h"

AssetData::AssetData(const Amount& amount, const std::string& assetClass, int id, int fromid) 
: m_amount(amount), m_assetClass(assetClass), m_id(id), m_fromid(fromid) 
{
}

AssetData::AssetData(const Amount& amount, const std::string& assetClass, int id) 
: m_amount(amount), m_assetClass(assetClass), m_id(id), m_fromid(id) 
{
}

Amount AssetData::getAmount() const 
{ 
   return m_amount; 
}

std::string AssetData::getAssetClass() const 
{ 
   return m_assetClass; 
}

int AssetData::getId() const
{
   return m_id;
}

int AssetData::getFromId() const
{
   return m_fromid;
}
