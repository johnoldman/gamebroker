//
// $Id: SkillGameServiceClient.cpp 3621 2007-12-14 12:38:23Z stephend $
//
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "MessageStack.h"
#include "ApiCommandRegisterGameInstance.h"
#include "ApiCommandDeregisterGameInstance.h"
#include "ApiCommandBeginUserSession.h"
#include "ApiCommandMakePayment.h"
#include "ApiCommandMakeSystemPayment.h"
#include "ApiCommandQuerySystemTotal.h"
#include "ApiCommandGetSystemData.h"
#include "ApiCommandGetUserData.h"
#include "ApiCommandSetUserData.h"
#include "SkillGameServiceClient.h"
#include "GameClientImpl.h"
#include "MQProducer.h"
#include "MQConsumer.h"
#include "Log4CppLogger.h"
#include "SglUninitialisedException.h"
#include "MQFactory.h"

using namespace SglGameAPI;

static GameClientImpl* m_gameImplInstance = 0;
static Logger* m_pLogger = 0;

void GameClient::initialise(const Config& config)
{
    m_pLogger = new Log4CppLogger(config.getLoggerPropertiesFile(), config.getLoggerName());
    std::auto_ptr<ProducerConsumerFactory> mqf(new MQFactory(config.getBrokerUri(), config.getServerQueueName(), config.getClientQueueName(), *m_pLogger));
    m_gameImplInstance = new GameClientImpl(mqf, *m_pLogger);
}

void GameClient::registerGameInstance(const std::string &gameName,
                                      const std::string &gameType,
                                      const std::string &gameSessionId,
                                      const std::string &gameTariffId)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->registerGameInstance(gameName, gameType, gameSessionId, gameTariffId);
   }
   else
   {
      throw SglUninitialisedException();
   }
}

void GameClient::deregisterGameInstance(const std::string &gameSessionId)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->deregisterGameInstance(gameSessionId);
   }
   else
   {
      throw SglUninitialisedException();
   }
}

void GameClient::beginUserSession(const std::string &messageId,
                      const std::string &gameSessionId,
                      long userId,
                      const std::string &userSessionId,
                      const std::string &tariffId,
                      long gameCost,
                      long gameFloorLimit,
                      BeginUserSessionCallback dataCallback,
                      BeginUserSessionExceptionCallback exceptionCallback)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->beginUserSession(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit, dataCallback, exceptionCallback);
   }
   else
   {
      throw SglUninitialisedException();
   }
}

void GameClient::endUserSession(const std::string &messageId,
		                const std::string &gameSessionId,
                                const std::string &userSessionId,
                                EndUserSessionCallback ackCallback,
                                EndUserSessionErrorCallback exceptionCallback)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->endUserSession(messageId, gameSessionId, userSessionId, ackCallback, exceptionCallback);
   }
   else
   {
      throw SglUninitialisedException();
   }
}

void GameClient::makePayment(const std::string &messageId,
                             const std::string &gameSessionId,
                             const std::string &fromUserSessionId,
                             const std::string &toUserSessionId,
                             long amount,
                             const std::string &eventCode,
                             MakePaymentCallback ackCallback,
                             MakePaymentErrorCallback exceptionCallback)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->makePayment(messageId, gameSessionId, fromUserSessionId, toUserSessionId, amount,   eventCode, ackCallback, exceptionCallback);
   }
   else
   {
      throw SglUninitialisedException();
   }
}

void GameClient::makeSystemPayment(const std::string &messageId,
		                             const std::string &gameSessionId,
		                             const std::string &userSessionId,
		                             bool payUser,
		                             long amount,
		                             const std::string &totalType,
                                   const std::string &totalClassification,
		                             const std::string &eventCode,
                                   MakeSystemPaymentCallback ackCallback,
                                   MakeSystemPaymentErrorCallback exceptionCallback)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->makeSystemPayment(messageId,
                                            gameSessionId,
                                            userSessionId,
                                            payUser,
                                            amount,
                                            totalType,
                                            totalClassification,
                                            eventCode,
                                            ackCallback,
                                            exceptionCallback);
   }
   else
   {
      throw SglUninitialisedException();
   }
}

void GameClient::querySystemTotal(const std::string &messageId,
		                             const std::string &gameSessionId,
		                             const std::string &totalType,
		                             void (*dataCallback)(const std::string &messageId,
		                                                  const std::string &gameSessionId,
		                                                  const std::string &totalType,
		                                                  long systemTotal),
		                             void (*exceptionCallback)(const std::string &messageId,
		                                                  const std::string &gameSessionId,
		                                                  const std::string &totalType,
                                                          enum SglGameAPI::SglServerExceptionType SglServerException,
                                                          std::string &SglServerExceptionDescription))
{
   (*dataCallback)(messageId, gameSessionId, totalType, 0);
}


void GameClient::getUserData(const std::string &messageId,
		                         const std::string &gameSessionId,
		                         long userId,
		                         const std::string &userDataType,
                             GetUserDataCallback dataCallback,
                             GetUserDataErrorCallback exceptionCallback)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->getUserData(messageId, gameSessionId, userId, userDataType, dataCallback, exceptionCallback);
   }
   else
   {
      throw SglUninitialisedException();
   }
}


void GameClient::setUserData(const std::string &messageId,
		                        const std::string &gameSessionId,
		                        long userId,
		                        const std::string &userDataType,
		                        const std::string &userData,
                                        SetUserDataCallback AckCallback,
                                        SetUserDataErrorCallback exceptionCallback)
{
   if (m_gameImplInstance)
   {
      m_gameImplInstance->setUserData(messageId,
                                   gameSessionId,
                                   userId,
                                   userDataType,
                                   userData,
                                   AckCallback,
                                   exceptionCallback);
   }
   else
   {
      throw SglUninitialisedException();
   }
}

std::string GameClient::getUniqueMessageId()
{
   if (m_gameImplInstance)
   {
      return m_gameImplInstance->getUniqueMessageId();
   }
   else
   {
      throw SglUninitialisedException();
   }
}

