//
// $Id: ServiceBroker.cpp 3706 2007-12-17 16:39:49Z stephend $
//
#include <stdexcept>
#include <memory>
#include <boost/bind.hpp>

#include "ServiceBroker.h"
#include "ApiCommandRegisterGameInstance.h"
#include "ApiCommandDeregisterGameInstance.h"
#include "ApiCommandBeginUserSession.h"
#include "ApiCommandEndUserSession.h"
#include "ApiCommandMakePayment.h"
#include "ApiCommandMakeSystemPayment.h"
#include "ApiCommandGetUserData.h"
#include "ApiCommandSetUserData.h"
//#include "MQConsumer.h"
#include "UserSessionId.h"
#include "XmlIterator.h"
#include "RegisterGameInstanceResponse.h"
#include "DeregisterGameInstanceResponse.h"
#include "BeginUserSessionResponse.h"
#include "EndUserSessionResponse.h"
#include "GetUserDataResponse.h"
#include "SetUserDataResponse.h"
#include "MakePaymentResponse.h"
#include "MakeSystemPaymentResponse.h"
#include "test/StubMessageConsumer.h"
#include "MessageAndResponse.h"
#include "MySqlPurseDb.h"
#include "ServiceBrokerResponse.h"
#include "PersistentStore.h"
#include "Logger.h"
#include "IPortalServiceProxy.h"
#include "ProducerConsumerFactory.h"

ServiceBroker::ServiceBroker(const ProducerConsumerFactory& producerFactory, GameMessageConsumer& consumer, PersistentStore& store, IKYRClient& kyr, Ledger& ledger, Logger& logger, IPortalServiceProxy& userService)
: m_producerFactory(producerFactory)
//, m_producer(m_producerFactory.createProducer(""))
, m_consumer(consumer)
, m_store(store)
, m_kyr(kyr)
, m_ledger(ledger)
, m_logger(logger)
, m_userService(userService)
{
}
//
// This function returns true if the message is a repeat - it also has the side effect of handling a repeated message
// so that the callers can have code of the form:
//
// if !messageRepeated(message)
//    process(message)
//
bool ServiceBroker::messageRepeated(const ApiCommandWithGameSessionId& message)
{
   std::auto_ptr<MessageAndResponse> mar = m_store.getMessage(message.getMessageId());
   bool retval = false;
   if(mar.get() == 0)
   {
      // Do Nothing - no previous copy of message.
   }
   else if(mar->getMessage() == message.getSerialised())
   {
      // return previous response
      //m_producer->sendMessage(mar->getResponse(), "");
      std::pair<ProducerMap::iterator, bool> insertWorked = m_producers.insert(std::make_pair(message.getGameSessionId(), boost::shared_ptr<GameMessageProducer>(m_producerFactory.createProducer(message.getGameSessionId()).release())));
      if (!insertWorked.second)
      {
         insertWorked.first = m_producers.find(message.getGameSessionId());
      }
      insertWorked.first->second->sendMessage(mar->getResponse(), message.getGameSessionId());
      retval = true;
   }
   else
   {
      std::pair<ProducerMap::iterator, bool> insertWorked = m_producers.insert(std::make_pair(message.getGameSessionId(), boost::shared_ptr<GameMessageProducer>(m_producerFactory.createProducer(message.getGameSessionId()).release())));
      if (!insertWorked.second)
      {
         insertWorked.first = m_producers.find(message.getGameSessionId());
      }
      insertWorked.first->second->sendMessage(mar->getResponse(), message.getGameSessionId());
      // notify corrupt message and return new message, stored message and stored response
      message.makeErrorResponse(SglGameAPI::SglServerCorruptMessage,
                                 std::string("New Message = " + message.getSerialised()
                                             + " compared to old Message = "
                                             + mar->getMessage() + " and old  Response = "
                                             + mar->getResponse()))->send(*insertWorked.first->second);
      retval = true;
   }
   return retval;
}

void ServiceBroker::gameInstanceDispatch(const ActionFunctionType& func, const ApiCommandWithGameSessionId& message)
{
   // begin game instance processing only passes through here...
   try
   {
      if(!messageRepeated(message))
      {
         processAndStore(func, message);
      }
   }
   catch (const std::exception& e)
   {
      m_logger.unexpectedException(e);
   }
   catch (...)
   {
      m_logger.unknownException("ServiceBroker::gameInstanceDispatch");
   }
}

void ServiceBroker::receive(const std::string& messageString, const time_t& eventTime)
{
    std::pair<std::string, XmlIterator> command = ApiCommand::parse(messageString);

   if (command.first == ApiCommandRegisterGameInstance::getTag())
   {
      ApiCommandRegisterGameInstance message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onRegisterGameInstance, this, message);
      gameInstanceDispatch(func, message);
   }
   else if (command.first == ApiCommandDeregisterGameInstance::getTag())
   {
      ApiCommandDeregisterGameInstance message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onDeregisterGameInstance, this, message);
      commonDispatch(func, message);
   }
   else if (command.first == ApiCommandBeginUserSession::getTag())
   {
      ApiCommandBeginUserSession message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onBeginUserSession, this, message);
      commonDispatch(func, message);
   }
   else if (command.first == ApiCommandEndUserSession::getTag())
   {
      ApiCommandEndUserSession message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onEndUserSession, this, message);
      commonDispatch(func, message);
   }
   else  if (command.first == ApiCommandMakePayment::getTag())
   {
      ApiCommandMakePayment message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onMakePayment, this, message);
      commonDispatch(func, message);
   }
   else  if (command.first == ApiCommandMakeSystemPayment::getTag())
   {
      ApiCommandMakeSystemPayment message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onMakeSystemPayment, this, message);
      commonDispatch(func, message);
   }
   else if (command.first == ApiCommandGetUserData::getTag())
   {
      ApiCommandGetUserData message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onGetUserData, this, message);
      commonDispatch(func, message);
   }
   else if (command.first == ApiCommandSetUserData::getTag())
   {
      ApiCommandSetUserData message(command.second, eventTime);
      boost::function0< std::auto_ptr<ServiceBrokerResponse> > func = boost::bind(&ServiceBroker::onSetUserData, this, message);
      commonDispatch(func, message);
   }
   else
   {
      throw std::runtime_error(std::string("Unrecognised message tag received:") + command.first + " " + messageString);
   }
}

void ServiceBroker::error(const std::string& message)
{
   m_logger.commsError(std::string("MQ error ") + message);
}

void ServiceBroker::commonDispatch(const ActionFunctionType& func, const ApiCommandWithGameSessionId& message)
{
   // common processing - all commands except for making a new game instance pass through here.
   // If this command is for a game instance which doesn't exist, try loading it from the backing store.
   try
   {
      if(!messageRepeated(message))
      {
         GameSessionId gameSessionId(message.getGameSessionId());
         SessionMap::const_iterator it = m_instances.find(gameSessionId);
         if (it == m_instances.end())
         {
            // game session not known locally - look for it in the persistant store (failover scenario)
            if(!m_store.gameInstanceExists(gameSessionId))
            {
               boost::shared_ptr<GameMessageProducer> producer(m_producerFactory.createProducer(message.getGameSessionId()).release());
               m_producers[message.getGameSessionId()] = producer;
               // game session dose not exist anywhere, so not a fail-over: report error
               message.makeErrorResponse(SglGameAPI::SglServerUnknownGameSessionId, gameSessionId.toString())->send(*producer);
            }
            else
            {
               // failover: recover game session from persistant store
               boost::shared_ptr<GameInstance> gi(new GameInstance(m_store, gameSessionId, m_kyr, m_ledger));
               boost::shared_ptr<GameMessageProducer> producer(m_producerFactory.createProducer(message.getGameSessionId()).release()); 
               m_instances.insert(std::make_pair(gameSessionId, GameSessionStructType(gi, producer)));

               // message is unique - process and store - doesn't throw so that message is always stored.
               processAndStore(func, message);
            }
         }
         else
         {
            processAndStore(func, message);
         }
      }
   }
   catch (const std::exception& e)
   {
      m_logger.unexpectedException(e);
   }
   catch (...)
   {
      m_logger.unknownException("ServiceBroker::dispatch");
   }
}

void ServiceBroker::processAndStore(const ActionFunctionType& func, const ApiCommandWithGameSessionId& message)
{
   std::auto_ptr<ServiceBrokerResponse> response = makeResponseMessage(func, message);
   m_store.storeMessage(message.getMessageId(), message.getSerialised(), response->getSerialised());
   // send back response
   SessionMap::iterator it = m_instances.find(GameSessionId(message.getGameSessionId()));
   if (it == m_instances.end())
   {
      ProducerMap::iterator pit = m_producers.find(message.getGameSessionId());
      if (pit == m_producers.end())
      {
         std::pair<ProducerMap::iterator, bool> insertWorked = m_producers.insert(std::make_pair(message.getGameSessionId(), boost::shared_ptr<GameMessageProducer>(m_producerFactory.createProducer(message.getGameSessionId()).release())));
         pit = insertWorked.first;
      }
      response->send(*pit->second);
   }
   else
   {
      response->send(*it->second.m_producer);
   }
}
//
// dispatch function - used to ensure common error and transactional handling for all messages
//
std::auto_ptr<ServiceBrokerResponse> ServiceBroker::makeResponseMessage(const ActionFunctionType& func, const ApiCommand& message)
{
   try
   {
      Transaction t = m_store.startTransaction();
      std::auto_ptr<ServiceBrokerResponse> message = func();
      t.commit();
      return message;
   }
   catch (const std::exception& e)
   {
      m_logger.unexpectedException(e);
      return message.makeErrorResponse(SglGameAPI::SglServerUnknownError, e.what());
   }
   catch (...)
   {
      m_logger.unknownException("ServiceBroker::makeResponseMessage");
      return message.makeErrorResponse(SglGameAPI::SglServerUnknownError, "Unknown exception");
   }
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onRegisterGameInstance(const ApiCommandRegisterGameInstance& message)
{
   GameSessionId gameSessionId(message.getGameSessionId());
   boost::shared_ptr<GameInstance> gi(new GameInstance(m_store, gameSessionId, message.getGameName(), message.getGameType(), message.getGameTariffId(), m_kyr, m_ledger));
   boost::shared_ptr<GameMessageProducer> producer(m_producerFactory.createProducer(message.getGameSessionId()).release()); 
   const std::pair<SessionMap::iterator, bool> insertWorked =
      m_instances.insert(std::make_pair(gameSessionId, GameSessionStructType(gi, producer)));

   // If the insert worked (ie item was unique), send ok else send error message indicating duplicate game session id.
   return insertWorked.second ? message.makeAckResponse() : message.makeErrorResponse(SglGameAPI::SglServerDuplicateGameSessionId, message.getGameSessionId());
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onDeregisterGameInstance(const ApiCommandDeregisterGameInstance& message)
{
   SessionMap::iterator it = m_instances.find(GameSessionId(message.getGameSessionId()));
   it->second.m_gameInstance->destroy();

   // clear any old producers - before adding the next producer
   retireOldProducers();

   m_producers.insert(std::make_pair(message.getGameSessionId(),it->second.m_producer));
   m_instances.erase(it);
   return message.makeAckResponse();
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onBeginUserSession(const ApiCommandBeginUserSession& message)
{
   SessionMap::iterator it = m_instances.find(GameSessionId(message.getGameSessionId()));
   const long purseTotal = it->second.m_gameInstance->beginUserSession(message.getUserId(), UserSessionId(message.getUserSessionId()), message.getTariffId(), message.getGameCost(), message.getFloorLimit());
   return message.makeAckResponse(purseTotal);
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onEndUserSession(const ApiCommandEndUserSession& message)
{
   SessionMap::iterator it = m_instances.find(GameSessionId(message.getGameSessionId()));
   it->second.m_gameInstance->endUserSession(UserSessionId(message.getUserSessionId()));
   return message.makeAckResponse();
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onMakePayment(const ApiCommandMakePayment& message)
{
   SessionMap::iterator it = m_instances.find(GameSessionId(message.getGameSessionId()));
   it->second.m_gameInstance->makePayment(UserSessionId(message.getFromUserSessionId()), UserSessionId(message.getToUserSessionId()), Amount(message.getAmount()), message.getEventCode(), message.getEventTime());
   return message.makeAckResponse();
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onMakeSystemPayment(const ApiCommandMakeSystemPayment& message)
{
   SessionMap::iterator it = m_instances.find(GameSessionId(message.getGameSessionId()));
   it->second.m_gameInstance->makeSystemPayment(UserSessionId(message.getUserSessionId()), message.getPayUser(), Amount(message.getAmount()), message.getTotalType(), message.getTotalClassification(), message.getEventCode(), message.getEventTime());
   return message.makeAckResponse();
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onGetUserData(const ApiCommandGetUserData& message)
{
   const std::string userData = m_userService.getUserData(message.getUserId(), message.getUserDataType());
   return message.makeAckResponse(userData);
}

std::auto_ptr<ServiceBrokerResponse> ServiceBroker::onSetUserData(const ApiCommandSetUserData& message)
{
   m_userService.setUserData(message.getUserId(), message.getUserDataType(), message.getUserData());
   return message.makeAckResponse();
}

void ServiceBroker::retireOldProducers()
{
   // search through m_producers deleting members that return isSafeToShutdown is true
   std::map<std::string, boost::shared_ptr<GameMessageProducer> >::iterator posn;
   for( posn = m_producers.begin(); posn != m_producers.end();)
   {
      if(posn->second->isSafeToShutdown())
      {
         m_producers.erase(posn++);
      } 
      else 
      {
         ++posn;
      }
   }
}
