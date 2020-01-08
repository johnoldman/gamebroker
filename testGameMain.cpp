//
// $Id: testGameMain.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include <string>
#include "Config.h"
#include "SkillGameServiceClient.h"

int main()
{
   std::string gameName("mettle");
   std::string gameType("death");
   std::string gameSessionId("gameSessionId");
 
   std::string brokerURI = "tcp://127.0.0.1:61616?wireFormat=openwire&transport.useAsyncSend=true";
   std::string serverQueue = "PURSE.SERVICE.SERVER";
   std::string clientQueue = "PURSE.SERVICE.CLIENT";
   std::string loggerPropertiesFile = "brokerlog.properties";
   std::string loggerName("TestGameClient");

   Config config(brokerURI, clientQueue, serverQueue, loggerPropertiesFile, loggerName);

   SglGameAPI::GameClient::initialise(config);

   SglGameAPI::GameClient::registerGameInstance(gameName,gameType,gameSessionId);
   
}
/*
   Config(const std::string& brokerUri, 
          const std::string& brokerSubject, 
          const std::string& loggerPropertiesFile, 
          const std::string& loggerName);
*/
