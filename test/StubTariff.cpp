//
// $Id: StubTariff.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "StubTariff.h"

void StubTariff::addMapping(const std::string& wintype, int startId, int endId)
{
   Mapping m(WinType(wintype), startId);

   m_mappings.insert(std::make_pair(m, endId));
}

std::auto_ptr<AssetClass> StubTariff::getAssetMapping(const WinType& w, const AssetClass& a) const
{
   Mapping m(w, a.getId());

   const std::map<Mapping, int>::const_iterator it = m_mappings.find(m);
   if (it != m_mappings.end())
   {
      return std::auto_ptr<AssetClass>(new AssetClass(it->second));
   }
   else
   {
      return std::auto_ptr<AssetClass>(0);
   }
}

size_t StubTariff::getMappingCount() const
{
   return m_mappings.size();
}
