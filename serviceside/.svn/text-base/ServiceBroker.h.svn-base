//
// $Id: ServiceBroker.h 3550 2007-12-13 14:26:04Z johno $
//
#ifndef SERVICEBROKER_H_
#define SERVICEBROKER_H_

#include <map>
#include <list>
#include <memory>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "ActiveMqConsumerHandler.h"
#include "GameInstance.h"
#include "KYRClient.h"
#include "XmlIterator.h"
#include "LedgerService.h"
#include "MySqlPurseDb.h"
#include "ServiceBrokerResponse.h"
#include "GameSessionId.h"
//#include "MQProducer.h"

class ApiCommandRegisterGameInstance;
class ApiCommandDeregisterGameInstance;
class ApiCommandBeginUserSession;
class ApiCommandEndUserSession;
class ApiCommandGetUserData;
class ApiCommandSetUserData;
class ApiCommandMakePayment;
class ApiCommandMakeSystemPayment;
class ApiCommand;
class ApiCommandWithGameSessionId;
//class GameMessageProducer;
class GameMessageConsumer;
class Logger;
class IPortalServiceProxy;
class ProducerConsumerFactory;

class ServiceBroker : public ActiveMqConsumerHandler
{
public:
   ServiceBroker(const ProducerConsumerFactory& producerFactory, GameMessageConsumer& consumer, PersistentStore& store, IKYRClient& kyr, Ledger& ledger, Logger& logger, IPortalServiceProxy& userService);
public:
   void receive(const std::string& message, const time_t& eventTime);
   void error(const std::string& message);
public:
   std::auto_ptr<ServiceBrokerResponse> onRegisterGameInstance(const ApiCommandRegisterGameInstance& command);
   std::auto_ptr<ServiceBrokerResponse> onDeregisterGameInstance(const ApiCommandDeregisterGameInstance& command);
   std::auto_ptr<ServiceBrokerResponse> onBeginUserSession(const ApiCommandBeginUserSession& command);
   std::auto_ptr<ServiceBrokerResponse> onEndUserSession(const ApiCommandEndUserSession& command);
   std::auto_ptr<ServiceBrokerResponse> onGetUserData(const ApiCommandGetUserData& command);
   std::auto_ptr<ServiceBrokerResponse> onSetUserData(const ApiCommandSetUserData& command);
   std::auto_ptr<ServiceBrokerResponse> onMakePayment(const ApiCommandMakePayment& message);
   std::auto_ptr<ServiceBrokerResponse> onMakeSystemPayment(const ApiCommandMakeSystemPayment& message);
private:
   typedef boost::function0< std::auto_ptr<ServiceBrokerResponse> > ActionFunctionType;
   void gameInstanceDispatch(const ActionFunctionType& func, const ApiCommandWithGameSessionId& message);
   void commonDispatch(const ActionFunctionType& func, const ApiCommandWithGameSessionId& message);
   bool messageRepeated(const ApiCommandWithGameSessionId& message);
   void processAndStore(const ActionFunctionType& func, const ApiCommandWithGameSessionId& message);
   std::auto_ptr<ServiceBrokerResponse>  processDispatch(const ActionFunctionType& func, const ApiCommand& message);
   std::auto_ptr<ServiceBrokerResponse> makeResponseMessage(const ActionFunctionType& func, const ApiCommand& message);
   void retireOldProducers();
   // structure to hold game instance and message producers (one-one relationship restraint)
   struct GameSessionStructType
   {
      GameSessionStructType(const boost::shared_ptr<GameInstance>& gameInstance, const boost::shared_ptr<GameMessageProducer>& messageProducer)
      : m_gameInstance(gameInstance)
      , m_producer(messageProducer)
      {}
      boost::shared_ptr<GameInstance> m_gameInstance;
      boost::shared_ptr<GameMessageProducer> m_producer;
   };
   typedef std::map<GameSessionId, GameSessionStructType> SessionMap;
   SessionMap m_instances;

   const ProducerConsumerFactory& m_producerFactory;
   //std::auto_ptr<GameMessageProducer> m_producer;
   GameMessageConsumer& m_consumer;
   PersistentStore& m_store;
   IKYRClient& m_kyr;
   Ledger& m_ledger;
   Logger& m_logger;
   IPortalServiceProxy& m_userService;
   typedef std::map<std::string, boost::shared_ptr<GameMessageProducer> > ProducerMap;
   ProducerMap m_producers;
};

#endif
