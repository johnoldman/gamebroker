//
// $Id: StubKYR.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef STUBKYR_H_
#define STUBKYR_H_

#include "IKYRClient.h"

#include <map>
#include <boost/shared_ptr.hpp>

class StubKYR : public IKYRClient
{
public:
   void addTariff(const std::string& name, boost::shared_ptr<ITariff> tariff);
public:
   boost::shared_ptr<ITariff> getTariff(const std::string& tariffid);
   int getUpdateCount() const { return 0; }
private:
   std::map<std::string, boost::shared_ptr<ITariff> > m_tariffs;
};

#endif

