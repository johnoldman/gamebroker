//
// $Id: GameClientImpl.h 3621 2007-12-14 12:38:23Z stephend $
//
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include "MessageStack.h"
#include "ApiCommandRegisterGameInstance.h"
#include "ApiCommandDeregisterGameInstance.h"
#include "ApiCallbackDefs.h"
#include "ApiCommandBeginUserSession.h"
#include "ApiCommandEndUserSession.h"
#include "ApiCommandGetUserData.h"
#include "ApiCommandSetUserData.h"
#include "ApiCommandMakePayment.h"
#include "ApiCommandMakeSystemPayment.h"
#include "GameMessageProducer.h"
#include "GameMessageConsumer.h"
#include "ActiveMqConsumerHandler.h"
#include "Queue.h"
#include "RegisterGameInstanceResponse.h"
#include "DeregisterGameInstanceResponse.h"
#include "MakePaymentResponse.h"
#include "MakeSystemPaymentResponse.h"
#include "MessageIdGenerator.h"
#include "ProducerConsumerFactory.h"

class BeginUserSessionResponse;
class EndUserSessionResponse;
class MakePaymentResponse;
class GetUserDataResponse;
class SetUserDataResponse;
class MessageIdGenerator;
class MQConsumer;
class Logger;
class MQConsumerFactory;
class MQFactory;

class GameClientImpl : public ActiveMqConsumerHandler
{
public:
   explicit GameClientImpl(std::auto_ptr<ProducerConsumerFactory> consumerFactory, Logger& logger);

   void registerGameInstance(const std::string &gameName,
                             const std::string &gameType,
                             const std::string &gameSessionId,
                             const std::string &gameTariffId);
   void deregisterGameInstance(const std::string &gameSessionId);
   void beginUserSession(const std::string &messageId,
                         const std::string &gameSessionId,
                         long userId,
                         const std::string &userSessionId,
                         const std::string &tariffId,
                         long gameCost,
                         long gameFloorLimit,
                         BeginUserSessionCallback dataCallback,
                         BeginUserSessionExceptionCallback exceptionCallback);
   void endUserSession(const std::string &messageId,
                       const std::string &gameSessionId,
                       const std::string &userSessionId,
                       EndUserSessionCallback ackCallback,
                       EndUserSessionErrorCallback exceptionCallback);
   void makePayment(const std::string &messageId,
                    const std::string &gameSessionId,
                    const std::string &fromUserSessionId,
                    const std::string &toUserSessionId,
                    long amount,
                    const std::string &eventCode,
                    MakePaymentCallback ackCallback,
                    MakePaymentErrorCallback errorcallback);

   void makeSystemPayment(const std::string &messageId,
                           const std::string &gameSessionId,
                           const std::string &userSessionId,
                           bool &payUser,
                           long amount,
                           const std::string &totalType,
                           const std::string &totalClassification,
                           const std::string &eventCode,
                           MakeSystemPaymentCallback ackCallback,
                           MakeSystemPaymentErrorCallback errorcallback);

   void getUserData(const std::string& messageId,
                    const std::string& gameSessionId,
                    long userId,
                    const std::string& userDataType,
                    GetUserDataCallback getUserDataCallback,
                    GetUserDataErrorCallback getUserDataExceptionCallback);
   void setUserData(const std::string &messageId,
                    const std::string &gameSessionId,
                    long userId,
                    const std::string &userDataType,
                    const std::string &userData,
                    SetUserDataCallback ackCallback,
                    SetUserDataErrorCallback errCallback);
   std::string getUniqueMessageId();

   // Implementation of ActiveMqConsumerHandler
   virtual void receive(const std::string& message, const time_t& eventTime);
   virtual void error(const std::string& message);
private:
   class BeginUserSession
   {
   public:
      BeginUserSession(const ApiCommandBeginUserSession& command, BeginUserSessionCallback callback, BeginUserSessionExceptionCallback errorCb)
      : m_command(command), m_okcb(callback), m_errorCb(errorCb)
      {
      }
      void callData(long purseTotal)
      {
         (*m_okcb)(m_command.getGameSessionId(),
                   m_command.getUserId(),
                   m_command.getUserSessionId(),
                   m_command.getGameCost(),
                   m_command.getFloorLimit(),
                   purseTotal);
      }
      void callError(enum SglGameAPI::SglServerExceptionType errorCode, const std::string& errorString)
      {
         (*m_errorCb)(m_command.getGameSessionId(),
                      m_command.getUserId(),
                      m_command.getUserSessionId(),
                      m_command.getGameCost(),
                      m_command.getFloorLimit(),
                      errorCode,
                      errorString);
      }
   private:
      ApiCommandBeginUserSession m_command;
      BeginUserSessionCallback m_okcb;
      BeginUserSessionExceptionCallback m_errorCb;
   };
   class EndUserSession
   {
   public:
      EndUserSession(const ApiCommandEndUserSession& command,
                     EndUserSessionCallback ackCallback,
                     EndUserSessionErrorCallback exceptionCallback)
      : m_command(command), m_okcb(ackCallback), m_errorcb(exceptionCallback)
      {
      }
      void callAck()
      {
         (*m_okcb)(m_command.getMessageId(),
                   m_command.getGameSessionId(),
                   m_command.getUserSessionId());
      }
      void callError(enum SglGameAPI::SglServerExceptionType errorCode, const std::string& errorString)
      {
         (*m_errorcb)(m_command.getMessageId(),
                      m_command.getGameSessionId(),
                      m_command.getUserSessionId(),
                      errorCode,
                      errorString);
      }
   private:
      ApiCommandEndUserSession m_command;
      EndUserSessionCallback m_okcb;
      EndUserSessionErrorCallback m_errorcb;
   };
   class MakePayment
   {
   public:
      MakePayment(const ApiCommandMakePayment& command,
                  MakePaymentCallback cb,
                  MakePaymentErrorCallback errcb)
      : m_command(command), m_okcb(cb), m_errorcb(errcb)
      {
      }
      void callAck()
      {
         (*m_okcb)(m_command.getMessageId(),
                   m_command.getGameSessionId(),
                   m_command.getFromUserSessionId(),
                   m_command.getToUserSessionId(),
                   m_command.getAmount(),
                   m_command.getEventCode());
      }
      void callError(enum SglGameAPI::SglServerExceptionType errorCode, const std::string& errorString)
      {
         (*m_errorcb)(m_command.getMessageId(),
                      m_command.getGameSessionId(),
                      m_command.getFromUserSessionId(),
                      m_command.getToUserSessionId(),
                      m_command.getAmount(),
                      m_command.getEventCode(),
                      errorCode,
                      errorString);
      }
   private:
      ApiCommandMakePayment m_command;
      MakePaymentCallback m_okcb;
      MakePaymentErrorCallback m_errorcb;
   };
   class MakeSystemPayment
   {
   public:
      MakeSystemPayment(const ApiCommandMakeSystemPayment& command,
                        MakeSystemPaymentCallback cb,
                        MakeSystemPaymentErrorCallback errcb)
      : m_command(command), m_okcb(cb), m_errorcb(errcb)
      {
      }
      void callAck()
      {
         (*m_okcb)(m_command.getMessageId(),
                   m_command.getGameSessionId(),
                   m_command.getUserSessionId(),
                   m_command.getPayUser(),
                   m_command.getAmount(),
                   m_command.getTotalType(),
                   m_command.getTotalClassification(),
                   m_command.getEventCode());
      }
      void callError(enum SglGameAPI::SglServerExceptionType errorCode, const std::string& errorString)
      {
         (*m_errorcb)(m_command.getMessageId(),
                      m_command.getGameSessionId(),
                      m_command.getUserSessionId(),
                      m_command.getPayUser(),
                      m_command.getAmount(),
                      m_command.getTotalType(),
                      m_command.getTotalClassification(),
                      m_command.getEventCode(),
                      errorCode,
                      errorString);
      }
   private:
      ApiCommandMakeSystemPayment m_command;
      MakeSystemPaymentCallback m_okcb;
      MakeSystemPaymentErrorCallback m_errorcb;
   };
   class GetUserData
   {
   public:
      GetUserData(const ApiCommandGetUserData& command,
                  GetUserDataCallback datacallback,
                  GetUserDataErrorCallback errorCallback)
      : m_command(command), m_okcb(datacallback), m_errorcb(errorCallback)
      {
      }
      void callData(const std::string& data)
      {
         (*m_okcb)(m_command.getMessageId(),
                   m_command.getGameSessionId(),
                   m_command.getUserId(),
                   m_command.getUserDataType(),
                   data);
      }
      void callError(enum SglGameAPI::SglServerExceptionType errorCode, const std::string& errorString)
      {
         (*m_errorcb)(m_command.getMessageId(),
                      m_command.getGameSessionId(),
                      m_command.getUserId(),
                      m_command.getUserDataType(),
                      errorCode,
                      errorString);
      }
   private:
      ApiCommandGetUserData m_command;
      GetUserDataCallback m_okcb;
      GetUserDataErrorCallback m_errorcb;
   };
   class SetUserData
   {
   public:
      SetUserData(const ApiCommandSetUserData& command,
                     SetUserDataCallback ackCallback,
                     SetUserDataErrorCallback errorCallback)
      : m_command(command), m_okcb(ackCallback), m_errorcb(errorCallback)
      {
      }
      void callAck()
      {
        (*m_okcb)(m_command.getMessageId(),
                  m_command.getGameSessionId(),
                  m_command.getUserId(),
                  m_command.getUserDataType(),
                  m_command.getUserData());
      }
      void callError(enum SglGameAPI::SglServerExceptionType errorCode, const std::string& errorString)
      {
         (*m_errorcb)(m_command.getMessageId(),
                      m_command.getGameSessionId(),
                      m_command.getUserId(),
                      m_command.getUserDataType(),
                      m_command.getUserData(),
                      errorCode,
                      errorString);
      }
   private:
      ApiCommandSetUserData m_command;
      SetUserDataCallback m_okcb;
      SetUserDataErrorCallback m_errorcb;
   };
   void onMessage(const RegisterGameInstanceResponse& message);
   void onMessage(const DeregisterGameInstanceResponse& message);
   void onMessage(const BeginUserSessionResponse& message);
   void onMessage(const EndUserSessionResponse& message);
   void onMessage(const MakePaymentResponse& message);
   void onMessage(const MakeSystemPaymentResponse& message);
   void onMessage(const GetUserDataResponse& message);
   void onMessage(const SetUserDataResponse& message);
private:
   std::auto_ptr<ProducerConsumerFactory> m_consumerFactory;
   std::auto_ptr<GameMessageProducer> m_producer;
   MessageIdGenerator theMessageIdGenerator;
   boost::mutex regGameInstMutex;
   MessageStack<ApiCommandRegisterGameInstance> m_registerGameCommands;
   boost::mutex deregGameInstMutex;
   MessageStack<ApiCommandDeregisterGameInstance> m_deregisterGameCommands;
   boost::mutex beginUserSessionMutex;
   MessageStack<BeginUserSession> m_beginUserSessionCommands;
   boost::mutex endUserSessionMutex;
   MessageStack<EndUserSession> m_endUserSessionCommands;
   boost::mutex makePaymentMutex;
   MessageStack<MakePayment> m_makePaymentCommands;
   boost::mutex makeSystemPaymentMutex;
   MessageStack<MakeSystemPayment> m_makeSystemPaymentCommands;
   boost::mutex getUserDataMutex;
   MessageStack<GetUserData> m_getUserDataCommands;
   boost::mutex setUserDataMutex;
   MessageStack<SetUserData> m_setUserDataCommands;

   Queue<RegisterGameInstanceResponse> q_registerGameResponses;
   Queue<DeregisterGameInstanceResponse> q_deregisterGameResponses;

   typedef std::map<std::string, boost::shared_ptr<GameMessageConsumer> > ConsumerMap;
   ConsumerMap m_messageConsumers;
   Logger& m_logger;
};
