//
// $Id: GameClientImpl.cpp 3706 2007-12-17 16:39:49Z stephend $
//
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "GameClientImpl.h"
#include "ApiCommandRegisterGameInstance.h"
#include "ApiCommandDeregisterGameInstance.h"
#include "ApiCommandBeginUserSession.h"
#include "ApiCommandEndUserSession.h"
#include "XmlIterator.h"
#include "RegisterGameInstanceResponse.h"
#include "DeregisterGameInstanceResponse.h"
#include "BeginUserSessionResponse.h"
#include "EndUserSessionResponse.h"
#include "ApiCommandMakePayment.h"
#include "MakePaymentResponse.h"
#include "MakeSystemPaymentResponse.h"
#include "GetUserDataResponse.h"
#include "SetUserDataResponse.h"
#include "Logger.h"
//#include "MQConsumer.h"
#include "ProducerConsumerFactory.h"

GameClientImpl::GameClientImpl(std::auto_ptr<ProducerConsumerFactory> consumerFactory, Logger& logger)
: m_consumerFactory(consumerFactory)
, m_producer(m_consumerFactory->createProducer(""))
, m_logger(logger)
{
}

void GameClientImpl::registerGameInstance(const std::string &gameName,
                                      const std::string &gameType,
                                      const std::string &gameSessionId,
                                      const std::string &gameTariffId)
{
   // Aquire mutex to prevent race conditions between adding sending message & getting response.
   boost::mutex::scoped_lock lock(regGameInstMutex);
   ApiCommandRegisterGameInstance registerCommand(gameName,gameType,gameSessionId, gameTariffId);

   // add a consumer for this game instance to the map
   const std::pair<ConsumerMap::iterator, bool> insertResult 
      = m_messageConsumers.insert(std::make_pair(gameSessionId, m_consumerFactory->createSharedConsumer(gameSessionId)));
   if (insertResult.second)
   {
      insertResult.first->second->setConsumer(this);
   }

   registerCommand.send(*m_producer);
   // Add the message after we send it, just in case it doesn't work.
   m_registerGameCommands.addMessage(registerCommand.getMessageId(), registerCommand);

   // release the mutex early
   lock.unlock();

   // get reply from response queue - when it arrives
   RegisterGameInstanceResponse response = q_registerGameResponses.getItem(registerCommand.getMessageId());
}

void GameClientImpl::deregisterGameInstance(const std::string &gameSessionId)
{
   // aquire the mutex
   boost::mutex::scoped_lock lock(deregGameInstMutex);
   ApiCommandDeregisterGameInstance command(gameSessionId);
   command.send(*m_producer);
   // Add the message after we send it, just in case it doesn't work.
   m_deregisterGameCommands.addMessage(command.getMessageId(), command);
   // release the mutex
   lock.unlock();
   // get reply from response queue - when it arrives
   DeregisterGameInstanceResponse response = q_deregisterGameResponses.getItem(command.getMessageId());
   // now remove the mq consumer
   std::map<std::string, boost::shared_ptr<GameMessageConsumer> >::iterator iter = m_messageConsumers.find(gameSessionId);
   m_messageConsumers.erase(gameSessionId);
}

void GameClientImpl::beginUserSession(const std::string &messageId,
                                  const std::string &gameSessionId,
                                  long userId,
                                  const std::string &userSessionId,
                                  const std::string &tariffId,
                                  long gameCost,
                                  long gameFloorLimit,
                                  BeginUserSessionCallback dataCallback,
                                  BeginUserSessionExceptionCallback exceptionCallback)
{
   // aquire the mutex
   boost::mutex::scoped_lock lock(beginUserSessionMutex);
   ApiCommandBeginUserSession beginCommand(messageId,
                                           gameSessionId,
                                           userId,
                                           userSessionId,
                                           tariffId,
                                           gameCost,
                                           gameFloorLimit);
   // Need to sent the command....
   beginCommand.send(*m_producer);
   BeginUserSession callback(beginCommand, dataCallback, exceptionCallback);
   m_beginUserSessionCommands.addMessage(messageId, callback);
   // release the mutex
   lock.unlock();
}

void GameClientImpl::endUserSession(const std::string &messageId,
                                    const std::string &gameSessionId,
                                    const std::string &userSessionId,
                                    EndUserSessionCallback ackCallback,
                                    EndUserSessionErrorCallback exceptionCallback)
{
   // aquire the mutex
   boost::mutex::scoped_lock lock(endUserSessionMutex);
   ApiCommandEndUserSession endCommand(messageId,
                                       gameSessionId,
                                       userSessionId);
   // Need to sent the command....
   endCommand.send(*m_producer);
   EndUserSession callback(endCommand, ackCallback, exceptionCallback);
   m_endUserSessionCommands.addMessage(messageId, callback);
   // release the mutex
   lock.unlock();
}

void GameClientImpl::makePayment(const std::string &messageId,
                                 const std::string &gameSessionId,
                                 const std::string &fromUserSessionId,
                                 const std::string &toUserSessionId,
                                 long amount,
                                 const std::string &eventCode,
                                 MakePaymentCallback ackCallback,
                                 MakePaymentErrorCallback exceptionCallback)
{
   boost::mutex::scoped_lock lock(makePaymentMutex);

   ApiCommandMakePayment makePaymentCommand(messageId,
                                            gameSessionId,
                                            fromUserSessionId,
                                            toUserSessionId,
                                            amount,
                                            eventCode);
   // Need to sent the command....
   makePaymentCommand.send(*m_producer);
   MakePayment callback(makePaymentCommand, ackCallback, exceptionCallback);
   m_makePaymentCommands.addMessage(messageId, callback);
}

   void GameClientImpl::makeSystemPayment(const std::string &messageId,
                                          const std::string &gameSessionId,
                                          const std::string &userSessionId,
                                          bool &payUser,
                                          long amount,
                                          const std::string &totalType,
                                          const std::string &totalClassification,
                                          const std::string &eventCode,
                                          MakeSystemPaymentCallback ackCallback,
                                          MakeSystemPaymentErrorCallback exceptionCallback)
{
   boost::mutex::scoped_lock lock(makeSystemPaymentMutex);

   ApiCommandMakeSystemPayment makeSystemPaymentCommand(messageId,
                                                        gameSessionId,
                                                        userSessionId,
                                                        payUser,
                                                        amount,
                                                        totalType,
                                                        totalClassification,
                                                        eventCode);
   // Need to sent the command....
   makeSystemPaymentCommand.send(*m_producer);
   MakeSystemPayment callback(makeSystemPaymentCommand, ackCallback, exceptionCallback);
   m_makeSystemPaymentCommands.addMessage(messageId, callback);
}

void GameClientImpl::getUserData(const std::string& messageId,
                    const std::string& gameSessionId,
                    long userId,
                    const std::string& userDataType,
                    GetUserDataCallback getUserDataCallback,
                    GetUserDataErrorCallback getUserDataExceptionCallback)
{
   boost::mutex::scoped_lock lock(getUserDataMutex);
   ApiCommandGetUserData command(messageId, gameSessionId, userId, userDataType);
   command.send(*m_producer);

   GetUserData callback(command, getUserDataCallback, getUserDataExceptionCallback);
   m_getUserDataCommands.addMessage(messageId, callback);
}

std::string GameClientImpl::getUniqueMessageId()
{
   return ++theMessageIdGenerator;
}

void GameClientImpl::onMessage(const RegisterGameInstanceResponse& message)
{
   // wait for mutex to be free & lock it.
   boost::mutex::scoped_lock lock(regGameInstMutex);
   ApiCommandRegisterGameInstance command = m_registerGameCommands.removeMessage(message.getMessageId());
   // put data on queue so that thread can pick it up.
   q_registerGameResponses.putItem(message.getMessageId(), message);
}

void GameClientImpl::onMessage(const DeregisterGameInstanceResponse& message)
{
   // wait for mutex to be free & lock it.
   boost::mutex::scoped_lock lock(deregGameInstMutex);
   ApiCommandDeregisterGameInstance command = m_deregisterGameCommands.removeMessage(message.getMessageId());
   // put data on queue so that thread can pick it up.
   q_deregisterGameResponses.putItem(message.getMessageId(), message);
}

void GameClientImpl::onMessage(const BeginUserSessionResponse& message)
{
   boost::mutex::scoped_lock lock(beginUserSessionMutex);

   BeginUserSession stackData = m_beginUserSessionCommands.removeMessage(message.getMessageId());

   if (message.isSuccess())
   {
      stackData.callData(message.getPurseTotal());
   }
   else
   {
      stackData.callError(message.getError(), message.getErrorString());
   }
}

void GameClientImpl::setUserData(const std::string &messageId,
                    const std::string &gameSessionId,
                    long userId,
                    const std::string &userDataType,
                    const std::string &userData,
                    SetUserDataCallback ackCallback,
                    SetUserDataErrorCallback errorCallback)
{
   boost::mutex::scoped_lock lock(setUserDataMutex);
   ApiCommandSetUserData command(messageId, gameSessionId, userId, userDataType, userData);
   command.send(*m_producer);

   SetUserData callback(command, ackCallback, errorCallback);
   m_setUserDataCommands.addMessage(messageId, callback);
}

void GameClientImpl::onMessage(const SetUserDataResponse& message)
{
   boost::mutex::scoped_lock lock(setUserDataMutex);

   SetUserData stackData = m_setUserDataCommands.removeMessage(message.getMessageId());

   if (message.isSuccess())
   {
      stackData.callAck();
   }
   else
   {
      stackData.callError(message.getError(), message.getErrorString());
   }
}

void GameClientImpl::onMessage(const EndUserSessionResponse& message)
{
   // wait for mutex to be free & lock it.
   boost::mutex::scoped_lock lock(endUserSessionMutex);
   EndUserSession stackData = m_endUserSessionCommands.removeMessage(message.getMessageId());
   if (message.isSuccess())
   {
      stackData.callAck();
   }
   else
   {
      stackData.callError(message.getError(), message.getErrorString());
   }
}

void GameClientImpl::onMessage(const MakePaymentResponse& message)
{
   // wait for mutex to be free & lock it.
   boost::mutex::scoped_lock lock(makePaymentMutex);
   MakePayment stackData = m_makePaymentCommands.removeMessage(message.getMessageId());
   if (message.isSuccess())
   {
      stackData.callAck();
   }
   else
   {
      stackData.callError(message.getError(), message.getErrorString());
   }
}

void GameClientImpl::onMessage(const MakeSystemPaymentResponse& message)
{
   // wait for mutex to be free & lock it.
   boost::mutex::scoped_lock lock(makeSystemPaymentMutex);
   MakeSystemPayment stackData = m_makeSystemPaymentCommands.removeMessage(message.getMessageId());
   if (message.isSuccess())
   {
      stackData.callAck();
   }
   else
   {
      stackData.callError(message.getError(), message.getErrorString());
   }
}

void GameClientImpl::onMessage(const GetUserDataResponse& message)
{
   // wait for mutex to be free & lock it.
   boost::mutex::scoped_lock lock(getUserDataMutex);
   GetUserData stackData = m_getUserDataCommands.removeMessage(message.getMessageId());
   if (message.isSuccess())
   {
      stackData.callData(message.getUserData());
   }
   else
   {
      stackData.callError(message.getError(), message.getErrorString());
   }
}
//
// Function wired into MQ consumer to receive messages
//
// If any of these items throws, then it is logged by the MQConsumer.
//
void GameClientImpl::receive(const std::string &message, const time_t& eventTime)
{
// XmlIterator iterator(message);
//
//  const std::string envelopeContents = iterator.extractXml("mq:envelope");
   const std::string envelopeContents = MQMessage::getEnvelopeContents(message);
   XmlIterator contents(envelopeContents);

   const std::string response = contents.extractString("response");
   if (response == RegisterGameInstanceResponse::getTag())
   {
      onMessage(RegisterGameInstanceResponse(contents));
   }
   else if (response == DeregisterGameInstanceResponse::getTag())
   {
      onMessage(DeregisterGameInstanceResponse(contents));
   }
   else if (response == BeginUserSessionResponse::getTag())
   {
      onMessage(BeginUserSessionResponse(contents));
   }
   else if (response == EndUserSessionResponse::getTag())
   {
      onMessage(EndUserSessionResponse(contents));
   }
   else if (response == GetUserDataResponse::getTag())
   {
      onMessage(GetUserDataResponse(contents));
   }
   else if (response == SetUserDataResponse::getTag())
   {
      onMessage(SetUserDataResponse(contents));
   }
   else if (response == MakePaymentResponse::getTag())
   {
      onMessage(MakePaymentResponse(contents));
   }
   else if (response == MakeSystemPaymentResponse::getTag())
   {
      onMessage(MakeSystemPaymentResponse(contents));
   }
   else
   {
      throw std::runtime_error(std::string("Bad message - unknown tag received ") + message);
   }
}

void GameClientImpl::error(const std::string& message)
{
   m_logger.commsError(std::string("MQ Error ") + message);
}
