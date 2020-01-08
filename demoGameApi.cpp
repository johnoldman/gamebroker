//
// $Id: demoGameApi.cpp 3773 2007-12-19 15:27:54Z johno $
//
#include <cstdio>
#include <string>
#include <iomanip>
#include <map>
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Config.h"
#include "SglUninitialisedException.h"
#include "SglStackDuplicateMessageIdException.h"
#include "SglStackUnknownMessageIdException.h"
#include "XmlIterator.h"
#include "MessageIdGenerator.h"
#include "SkillGameServiceClient.h"
#include "CPrefsFile.h"

// TODO
// get xml from mq:envelope's in file, rather than entire file, to reduce buffer size requirements
// add cs parser

bool replyErrorDetected = false;
bool apiErrorDetected = false;

std::string getServerErrorCode(SglGameAPI::SglServerExceptionType sglServerException)
{
   // bit of robustness if more exceptions added
   std::string serverSideErrorCodes[] =
   {
      std::string("SglSuccess"),
      std::string("SglServerUnknownGameSessionId"),
      std::string("SglServerGameClosed"),
      std::string("SglServerUnknownUserId"),
      std::string("SglServerUnknownUserSessionId"),
      std::string("SglServerInsufficientFunds"),
      std::string("SglServerDuplicateUserSessionId"),
      std::string("SglServerDuplicateGameSessionId"),
      std::string("SglServerFundsRemaining"),
      std::string("SglServerUnknownEventCode"),
      std::string("SglServerUnknownDataType"),
      std::string("SglServerCorruptMessage"),
      std::string("SglNotInitialised"),
      std::string("SglServerUnknownError")
   };

   const long maxSqlExceptions = (SglGameAPI::SglServerUnknownError + 1);

   if(sglServerException < maxSqlExceptions)
   {
      replyErrorDetected = true;
      return serverSideErrorCodes[sglServerException];
   }
   else
   {
      replyErrorDetected = true;
      return "ERROR: unrecognised sglServerException value: " + sglServerException;
   }
}

// API callbacks ...
static void beginUserSessionCallback(const std::string &gameSessionId,
                                     long userId,
                                     const std::string &userSessionId,
                                     long gameCost,
                                     long gameFloorLimit,
                                     long purseTotal)
{
   std::cout << "beginUserSessionCallback with lastPurseTotal= " << purseTotal << std::endl;
}

static void beginUserSessionExceptionCallback(const std::string &gameSessionId,
                                              long userId,
                                              const std::string &userSessionId,
                                              long gameCost,
                                              long gameFloorLimit,
                                              enum SglGameAPI::SglServerExceptionType sglServerException,
                                              const std::string& sglServerExceptionDescription)
{
   std::cerr << "beginUserSessionExceptionCallback: " << getServerErrorCode(sglServerException) << " : " << sglServerExceptionDescription << std::endl;
   replyErrorDetected = true;
}

static void endUserSessionCallback(const std::string &messageId,
                                   const std::string &gameSessionId,
                                   const std::string &userSessionId)
{
   std::cout << "endUserSessionCallback acknowledge for [" << gameSessionId << ":" << userSessionId << "]" << std::endl;
}

static void endUserSessionExceptionCallback(const std::string &messageId,
                                   const std::string &gameSessionId,
                                   const std::string &userSessionId,
                                   enum SglGameAPI::SglServerExceptionType sglServerException,
                                   const std::string& sglServerExceptionDescription)
{
   std::cerr << "endUserSessionExceptionCallback: " << getServerErrorCode(sglServerException) << " : " << sglServerExceptionDescription << std::endl;
   replyErrorDetected = true;
}

static void makePaymentCallback(const std::string &messageId,
                      const std::string &gameSessionId,
                      const std::string &fromUserSessionId,
                      const std::string &toUserSessionId,
                      long amount,
                      const std::string &eventCode)
{
   std::cout << "makePaymentCallback " << messageId << " acknowledge" << std::endl;
}

static void makePaymentExceptionCallback(const std::string &messageId,
                               const std::string &gameSessionId,
                               const std::string &fromUserSessionId,
                               const std::string &toUserSessionId,
                               long amount,
                               const std::string &eventCode,
                               enum SglGameAPI::SglServerExceptionType sglServerException,
                               const std::string &SglServerExceptionDescription)
{
   std::cerr << "makePaymentExceptionCallback: " << getServerErrorCode(sglServerException) << " : " <<  SglServerExceptionDescription << std::endl;
   replyErrorDetected = true;
}

// api calls
void callRegisterGameInstance(const std::string &gameName,
                              const std::string &gameType,
                              const std::string &gameSessionId,
                              const std::string &gameTariffId)
{
   try
   {
      SglGameAPI::GameClient::registerGameInstance(gameName,gameType,gameSessionId,gameTariffId);
      std::cerr << "registered " << gameSessionId << std::endl;
   }
   catch(const SglUninitialisedException& e)
   {
      std::cerr << "registerGameInstance: SglUninitialisedException" << std::endl;
      apiErrorDetected = true;
   }
   catch(const std::exception& e)
   {
      std::cerr << "registerGameInstance (" << gameSessionId << ") std::exception " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(...)
   {
      std::cerr << "registerGameInstance threw an unknown exception" << std::endl;
      apiErrorDetected = true;
   }
}

void callDeregesterGameInstance(const std::string &gameSessionId)
{
   try
   {
      SglGameAPI::GameClient::deregisterGameInstance(gameSessionId);
      std::cerr << "deregistered " << gameSessionId << std::endl;
   }
   catch(const SglUninitialisedException)
   {
      std::cerr << "deregisterGameInstance: SglUninitialisedException " << std::endl;
      apiErrorDetected = true;
   }
   catch(const std::exception& e)
   {
      std::cerr << "deregisterGameInstance (" << gameSessionId << ") exception " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(...)
   {
      std::cerr << "deregisterGameInstance: non-std exception" << std::endl;
      apiErrorDetected = true;
   }
} 

void callBeginUserSession(const std::string &messageId,
                          const std::string &gameSessionId,
                          long userId,
                          const std::string &userSessionId,
                          const std::string &tarrifId,
                          long gameCost,
                          long gameFloorLimit)
{
   try
   {
      SglGameAPI::GameClient::beginUserSession(messageId,
                              gameSessionId,
                              userId,
                              userSessionId,
                              tarrifId,
                              gameCost,
                              gameFloorLimit,
                              beginUserSessionCallback,
                              beginUserSessionExceptionCallback);
   }
   catch(const SglStackUnknownMessageIdException& e)
   {
      std::cerr << "beginUserSession: SglStackUnknownMessageIdException " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(const SglStackDuplicateMessageIdException& e)
   {
      std::cerr << "beginUserSession: SglStackDuplicateMessageIdException " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(const SglUninitialisedException)
   {
      std::cerr << "beginUserSession: SglUninitialisedException " << std::endl;
      apiErrorDetected = true;
   }
   catch( const std::exception& e )
   {
      std::cerr << "beginUserSession (" << userSessionId << ") exception: " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(...)
   {
      std::cerr << "beginUserSession: non-std exception" << std::endl;
      apiErrorDetected = true;
   }
}

void callEndUserSession(const std::string &messageId,
                        const std::string &gameSessionId,
                        const std::string &userSessionId)
{
   try
   {
      SglGameAPI::GameClient::endUserSession(messageId,
                              gameSessionId,
                              userSessionId,
                              endUserSessionCallback,
                              endUserSessionExceptionCallback);
   }
   catch(const SglStackUnknownMessageIdException& e)
   {
      std::cerr << "endUserSession: SglStackUnknownMessageIdException "<< e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(const SglStackDuplicateMessageIdException& e)
   {
      std::cerr << "endUserSession: SglStackDuplicateMessageIdException "<< e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(const SglUninitialisedException)
   {
      std::cerr << "endUserSession: SglUninitialisedException " << std::endl;
      apiErrorDetected = true;
   }
   catch(const std::exception& e)
   {
      std::cerr << "endUserSession (" << userSessionId << ") exception: " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(...)
   {
      std::cerr << "endUserSession: non-std exception" << std::endl;
      apiErrorDetected = true;
   }
}

void callMakePayment(const std::string &messageId,
                     const std::string &gameSessionId,
                     const std::string &fromUserSessionId,
                     const std::string &toUserSessionId,
                     long amount,
                     const std::string &eventCode)
{
   try
   {
      SglGameAPI::GameClient::makePayment(messageId,
                              gameSessionId,
                              fromUserSessionId,
                              toUserSessionId,
                              amount,
                              eventCode,
                              makePaymentCallback,
                              makePaymentExceptionCallback);
   }
   catch(const SglStackUnknownMessageIdException& e)
   {
      std::cerr << "makePayment: SglStackUnknownMessageIdException " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(const SglStackDuplicateMessageIdException& e)
   {
      std::cerr << "makePayment: SglStackDuplicateMessageIdException " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(const SglUninitialisedException)
   {
      std::cerr << "makePayment: SglUninitialisedException exception " << std::endl;
      apiErrorDetected = true;
   }
   catch(const std::exception& e)
   {
      std::cerr << "makePayment(" << messageId << "): exception " << e.what() << std::endl;
      apiErrorDetected = true;
   }
   catch(...)
   {
      std::cerr << "makePayment: non-std exception" << std::endl;
      apiErrorDetected = true;
   }
}

// wrapper for demonstrating the SgiGameApi
int main(int argc, char **argv)
{
   const long expectedArgs = 3;
   const long extraArgs = (argc - expectedArgs);
   if(argc < expectedArgs)
   {
      std::cerr << "Usage: " << argv[0]
                << " <config xml file> <test script xml file>" << std::endl;
      exit(1);
   }
   else if(argc > expectedArgs)
   {
      std::cout << "WARNING: " << extraArgs << " extra argument" 
                << (extraArgs == 1 ? "" : "s") << " ignored" << std::endl;
   }

   MessageIdGenerator messIdGen;

   try
   {
      // read in the configuration
      std::auto_ptr<CPrefsFile> configFile = std::auto_ptr<CPrefsFile>(new CPrefsFile(argv[1]));
      std::string configBuf;
      configFile->Read(configBuf);
      XmlIterator configIter(configBuf);
      const std::string configData = configIter.extractXml("apiservicebroker");
      XmlIterator configDataIt(configData);
      const std::string brokerURI = configDataIt.extractString("brokeruri");
      const std::string clientQueue = configDataIt.extractString("clientqueue");
      const std::string serverQueue = configDataIt.extractString("serverqueue");
      const std::string loggerPropertiesFile = configDataIt.extractString("loggerproperties");
      const std::string loggerName = configDataIt.extractString("loggername");

      // initialise the game api
      Config config(brokerURI, clientQueue, serverQueue, loggerPropertiesFile, loggerName);
      SglGameAPI::GameClient::initialise(config);

      // read in xml file
      std::auto_ptr<CPrefsFile> scriptFile = std::auto_ptr<CPrefsFile>(new CPrefsFile(argv[2]));
      std::string scriptBuf;
      scriptFile->Read(scriptBuf);
      XmlIterator wholeScriptIter(scriptBuf);
      // get the script data
      const std::string scriptDataContents = wholeScriptIter.extractXml("apitestscript");
      XmlIterator scriptDataIter(scriptDataContents);

      std::string commandRequest("");
      bool useAutoGameSessionId = false;
      bool useAutoMessageId     = false;
      bool useStopOnReplyError  = false;
      bool useStopOnApiError    = false;
      std::string autoGeneratedGameSessionId;
      long repeat_next_command = 1L;

      // loop through all the mq::envelopes
      do
      {
         // get an envelope data
         const std::string envelopeContents = scriptDataIter.extractXml("mq:envelope");
         XmlIterator envelopeIter(envelopeContents);

         commandRequest = envelopeIter.extractString("command");

         if(commandRequest == "CONFIGURE")
         {
            // configure request
            const std::string parameters = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            const std::string autoGameSessionId = parameterIter.extractString("autoGameSessionId");
            autoGameSessionId == "true" ? useAutoGameSessionId = true : useAutoGameSessionId = false;
            std::cout << "CONFIGURE: toggling autoGameSessionId to " << autoGameSessionId << std::endl;
            if(useAutoGameSessionId)
            {
               std::cout << "CONFIGURE:    note that all subsequent gameSessionId parameters will be ignored" << std::endl;
            }
            const std::string autoMessageId = parameterIter.extractString("autoMessageId");
            autoMessageId == "true" ? useAutoMessageId = true : useAutoMessageId = false;
            std::cout << "CONFIGURE: toggling autoMessageId to " << autoMessageId << std::endl;
            if(useAutoGameSessionId)
            {
               std::cout << "CONFIGURE:    note that all subsequent messageId parameters will be ignored" << std::endl;
            }
            const std::string stopOnApiError = parameterIter.extractString("stopOnApiError");
            stopOnApiError == "true" ? useStopOnApiError = true : useStopOnApiError = false;
            std::cout << "CONFIGURE: toggling stop-on-api-error to " << stopOnApiError << std::endl;
            const std::string stopOnReplyError = parameterIter.extractString("stopOnReplyError");
            stopOnReplyError == "true" ? useStopOnReplyError = true : useStopOnReplyError = false;
            std::cout << "CONFIGURE: toggling stop-on-reply-error to " << stopOnApiError << std::endl;
         }
         else if(commandRequest == "RGI")
         {
            // register a game instance
            const std::string parameters = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            const std::string gameName     = parameterIter.extractString("gameName");
            const std::string gameType     = parameterIter.extractString("gameType");
            std::string gameSessionId      = parameterIter.extractString("gameSessionId");
            if(useAutoGameSessionId)
            {
               autoGeneratedGameSessionId = ++messIdGen;
               gameSessionId = autoGeneratedGameSessionId;
            }
            const std::string gameTariffId = parameterIter.extractString("gameTariffId");
            for(int i=0; i<repeat_next_command; i++)
            {
               if(useAutoGameSessionId)
               {
                  autoGeneratedGameSessionId = ++messIdGen;
                  gameSessionId = autoGeneratedGameSessionId;
               }
               callRegisterGameInstance(gameName, gameType, gameSessionId, gameTariffId);
            }
            repeat_next_command = 1;
         }
         else if(commandRequest == "DGI")
         {
            // deregister a game instance
            const std::string parameters = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            std::string gameSessionId = parameterIter.extractString("gameSessionId");
            if(useAutoGameSessionId) gameSessionId = autoGeneratedGameSessionId;
            for(int i=0; i<repeat_next_command; i++)
            {
               callDeregesterGameInstance(gameSessionId);
            }
            repeat_next_command = 1;
         }
         else if(commandRequest == "BUS")
         {
            // begin user session
            const std::string parameters = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            std::string messageId                  = parameterIter.extractString("messageId");
            if(useAutoMessageId) messageId         = ++messIdGen;
            std::string gameSessionId              = parameterIter.extractString("gameSessionId");
            if(useAutoGameSessionId) gameSessionId = autoGeneratedGameSessionId;
            long userId                            = parameterIter.extractLong("userId");
            const std::string userSessionId        = parameterIter.extractString("userSessionId");
            const std::string tariffId             = parameterIter.extractString("tariffId");
            long gameCost                          = parameterIter.extractLong("gameCost");
            long gameFloorLimit                    = parameterIter.extractLong("gameFloorLimit");
            for(int i=0; i<repeat_next_command; i++)
            {
               if(useAutoMessageId) messageId = ++messIdGen;
               callBeginUserSession(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
            }
            repeat_next_command = 1;
         }
         else if(commandRequest == "EUS")
         {
            // end user session
            const std::string parameters           = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            std::string messageId                  = parameterIter.extractString("messageId");
            std::string gameSessionId              = parameterIter.extractString("gameSessionId");
            if(useAutoGameSessionId) gameSessionId = autoGeneratedGameSessionId;
            const std::string userSessionId        = parameterIter.extractString("userSessionId");
            for(int i=0; i<repeat_next_command; i++)
            {
               if(useAutoMessageId) messageId = ++messIdGen;
               callEndUserSession(messageId, gameSessionId, userSessionId);
            }
            repeat_next_command = 1;
         }
         else if(commandRequest == "MPY")
         {
            // make payment
            const std::string parameters = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            std::string messageId                  = (useAutoMessageId) ? ++messIdGen : parameterIter.extractString("messageId");
            std::string gameSessionId              = (useAutoGameSessionId) ? autoGeneratedGameSessionId : parameterIter.extractString("gameSessionId");
            const std::string fromUserSessionId    = parameterIter.extractString("fromUserSessionId");
            const std::string toUserSessionId      = parameterIter.extractString("toUserSessionId");
            long amount                            = parameterIter.extractLong("amount");
            const std::string eventCode            = parameterIter.extractString("eventCode");
            for(int i=0; i<repeat_next_command; i++)
            {
               callMakePayment(messageId, gameSessionId, fromUserSessionId, toUserSessionId, amount, eventCode);
               if(useAutoMessageId) messageId = ++messIdGen;
            }
            repeat_next_command = 1;
         }
         else if(commandRequest == "REPEAT_NEXT_COMMAND")
         {
            // loop version of make payment
            const std::string parameters = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            repeat_next_command = parameterIter.extractLong("repeats");
            //std::cout << "REPEAT_NEXT_COMMAND: " << repeat_next_command << " time" << (repeat_next_command < 2 ? "" : "s") << std::endl;            
         }
         else if(commandRequest == "WAIT")
         {
            // request to wait
            const std::string parameters = envelopeIter.extractXml("parameters");
            XmlIterator parameterIter(parameters);
            long seconds = parameterIter.extractLong("seconds");
            std::cout << "WAITING: for " << seconds << " second" << (seconds  < 2 ? "" : "s") << std::endl;
            sleep(seconds);
         }
         else if(commandRequest == "END")
         {
            // deetct end of script
            std::cout << "Script Terminator Found" << std::endl;
         }
         else
         {
            std::cerr << "Script error: " << commandRequest << std::endl;
         }
      }while((commandRequest != "END") 
            && !((useStopOnApiError == true)&&(apiErrorDetected == true))
            && !((useStopOnReplyError == true)&&(replyErrorDetected == true)) );
   }
   catch(const std::exception& e)
   {
      std::cerr << argv[0] << " threw exception " << e.what() << std::endl;
   }
   catch( ... )
   {
      std::cerr << argv[0] << " threw a non-std exception" << std::endl;
   }

   // allow for the broker to acknowledge, to ensure that the message gets cleared from the queue
   //sleep(1);

   return 0;
}


