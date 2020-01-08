//
// $Id: PurseData.h 3484 2007-12-12 11:57:57Z stephend $
//
#include <string>

#include "UserSessionId.h"

class PurseData
{
public:
   PurseData(const std::string& key, long userId, const std::string& tariffid, const std::string& id);
   std::string getKey() const { return m_key; }
   long getUserId() const { return m_userId; }
   std::string getTariffId() const { return m_tariffid; }
   std::string getId() const { return m_id; }
   //bool operator<(const PurseData& other) const;
private:
   std::string m_key;
   long m_userId;
   std::string m_tariffid;
   std::string m_id;
};
