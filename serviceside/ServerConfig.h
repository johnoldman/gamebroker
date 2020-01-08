//
// $Id: ServerConfig.h 3484 2007-12-12 11:57:57Z stephend $
//
#include "Config.h"

class ServerConfig : public Config
{
public:
   ServerConfig(const std::string& brokerUri, const std::string& clientQueueName, const std::string& serverQueueName, const std::string& loggerPropertiesFile, const std::string& loggerName,
                const std::string& dbUri, const std::string& ledgerUri, const std::string& kyrUri);
   std::string getDbUri() const { return m_dbUri; }
   std::string getLedgerUri() const { return m_ledgerUri; }
   std::string getKyrUri() const { return m_kyrUri; }
private:
   const std::string m_dbUri;
   const std::string m_ledgerUri;
   const std::string m_kyrUri;
};
