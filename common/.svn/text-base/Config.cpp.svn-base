//
// $Id: Config.cpp 3773 2007-12-19 15:27:54Z johno $
//
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include "XmlIterator.h"
#include "Config.h"
#include "CPrefsFile.h"

Config::Config(const std::string& brokerUri, const std::string& clientQueueName, const std::string& serverQueueName, const std::string& loggerPropertiesFile, const std::string& loggerName)
: m_brokerUri(brokerUri)
, m_clientQueueName(clientQueueName)
, m_serverQueueName(serverQueueName)
, m_loggerPropertiesFile(loggerPropertiesFile)
, m_loggerName(loggerName)
{}

Config::Config(const std::string& xmlFileName)
{
   std::auto_ptr<CPrefsFile> configFile = std::auto_ptr<CPrefsFile>(new CPrefsFile(xmlFileName));
   std::string configBuf;
   configFile->Read(configBuf);
   XmlIterator configIter(configBuf);
   std::string configData = configIter.extractXml("apiservicebroker");
   XmlIterator configDataIt(configData);
   m_brokerUri = configDataIt.extractString("brokeruri");
   m_clientQueueName = configDataIt.extractString("clientqueue");
   m_serverQueueName = configDataIt.extractString("serverqueue");
   m_loggerPropertiesFile = configDataIt.extractString("loggerproperties");
   m_loggerName = configDataIt.extractString("loggername");
}
