//
// $Id: ZinixEngineMain.cpp 3774 2007-12-19 15:48:42Z johno $
//

//#include <cstdio>

#include <boost/shared_ptr.hpp>

#include "MQConsumer.h"
#include "MQProducer.h"
#include "MQFactory.h"
#include "Log4CppLogger.h"
#include "serviceside/ServiceBroker.h"
#include "serviceside/ServerConfig.h"
#include "serviceside/MySqlPurseDb.h"
#include "XmlIterator.h"
#include "SglUserService.h"
#include "CPrefsFile.h"

int main(int argc,char **argv)
{
   if (argc < 2)
   {
      std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
      exit(1);
   }
   try 
   {
      std::auto_ptr<CPrefsFile> configFile = std::auto_ptr<CPrefsFile>(new CPrefsFile(argv[1]));
      std::string buf;
      configFile->Read(buf);
      XmlIterator it(buf);
      const std::string data = it.extractXml("purseservicebroker");
      XmlIterator datait(data);
      const std::string brokerURI = datait.extractString("brokeruri");
      const std::string serverQueue = datait.extractString("serverqueue");
      const std::string clientQueue = datait.extractString("clientqueue");
      const std::string loggerPropertiesFile = datait.extractString("loggerproperties");
      const std::string loggerName = datait.extractString("loggername");
      const std::string dbdata = datait.extractXml("pursedb");
      XmlIterator dbit(dbdata);
      const std::string dbhostname = dbit.extractString("hostname");
      const std::string dbname = dbit.extractString("dbname");
      const std::string dbusername = dbit.extractString("username");
      const std::string dbpassword = dbit.extractString("password");
      const std::string ledgerUri = datait.extractString("ledgeruri");
      const std::string kyrdata = datait.extractXml("kyrservice");
      XmlIterator kyrit(kyrdata);
      const std::string kyrUri = kyrit.extractString("uri");
      const int kyrport = kyrit.extractLong("localport");
      const int kyrpoll = kyrit.extractLong("polltime");
      const int kyrtimeout = kyrit.extractLong("timeout");
      const std::string userServiceEndpoint = datait.extractString("userserviceuri");

   //ServerConfig sc(brokerURI, clientQueue, serverQueue, loggerPropertiesFile, loggerName, dbUri, ledgerUri, kyrUri);
      Log4CppLogger logger(loggerPropertiesFile, loggerName);
      //MQProducer producer(brokerURI, clientQueue, serverQueue, logger);
      MQConsumer consumer(brokerURI, serverQueue, logger);
      MQFactory producerFactory(brokerURI, clientQueue, serverQueue, logger);
      MySqlPurseDb store(dbhostname, dbname, dbusername, dbpassword);
      KYRClient kyr(kyrUri, kyrport, kyrpoll, kyrtimeout);
      LedgerService ledger(ledgerUri);
      SglUserService userservice(userServiceEndpoint);
      ServiceBroker sb(producerFactory, consumer, store, kyr, ledger, logger, userservice);
      consumer.setConsumer(&sb);
      // Not sure about this loop - should we be allowed to closedown somehow?
      while (1)
      {
         sleep(100000);
      }
   }
   catch (const std::exception& e)
   {
      std::cerr << argv[0] << " : Exiting due to  " << e.what() << std::endl;
      exit(1);
   }
   return 0;
}
