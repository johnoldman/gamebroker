//
// $Id: StubKYR.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "StubKYR.h"
#include "StubTariff.h"

void StubKYR::addTariff(const std::string& name, boost::shared_ptr<ITariff> tariff)
{
   m_tariffs.insert(std::make_pair(name, tariff));
}

boost::shared_ptr<ITariff> StubKYR::getTariff(const std::string& tariffid)
{
   std::map<std::string, boost::shared_ptr<ITariff> >::const_iterator it = m_tariffs.find(tariffid);
   if (it != m_tariffs.end())
   {
      return it->second;
   }
   else
   {
      return boost::shared_ptr<ITariff>(new StubTariff);
   }
}
