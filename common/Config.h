#ifndef _CONFIG_H_
#define _CONFIG_H_

//
// $Id: Config.h 3773 2007-12-19 15:27:54Z johno $
//
#include <string>

class Config
{
public:
   Config(const std::string& brokerUri, const std::string& clientQueueName, const std::string& serverQueueName, const std::string& loggerPropertiesFile, const std::string& loggerName);
   Config(const std::string& xmlFileName);
   std::string getBrokerUri() const { return m_brokerUri; }
   std::string getClientQueueName() const { return m_clientQueueName; }
   std::string getServerQueueName() const { return m_serverQueueName; }
   std::string getLoggerPropertiesFile() const { return m_loggerPropertiesFile; }
   std::string getLoggerName() const { return m_loggerName; }
private:
   std::string m_brokerUri;
   std::string m_clientQueueName;
   std::string m_serverQueueName;
   std::string m_loggerPropertiesFile;
   std::string m_loggerName;
};

#endif
