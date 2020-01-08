//
// $Id: ServerConfig.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "ServerConfig.h"

ServerConfig::ServerConfig(const std::string& brokerUri, const std::string& clientQueueName, const std::string& serverQueueName, 
                           const std::string& loggerPropertiesFile, const std::string& loggerName,
                           const std::string& dbUri, const std::string& ledgerUri, const std::string& kyrUri)
: Config(brokerUri, clientQueueName, serverQueueName, loggerPropertiesFile, loggerName)
, m_dbUri(dbUri), m_ledgerUri(ledgerUri), m_kyrUri(kyrUri)
{
}
