//
// $Id: MQProducer.h 3537 2007-12-13 12:19:30Z johno $
//
#ifndef MESSAGE_PRODUCER_HPP_
#define MESSAGE_PRODUCER_HPP_


#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/Runnable.h>
#include <activemq/concurrent/CountDownLatch.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Integer.h>
#include <activemq/util/Config.h>
#include <activemq/util/Date.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <iostream>

using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::concurrent;
//using namespace cms;
using namespace std;

class Logger;

#include "GameMessageProducer.h"

// Set the URI to point to the IPAddress of your broker.
// add any optional params to the url to enable things like
// tightMarshalling or tcp logging etc.  See the CMS website for
// a full list of configuration options.
//
//  http://activemq.apache.org/cms/
//
// Wire Foormat Options:
// =====================
// Use either stomp or openwire, the default ports are different for each
//
// Examples:
//    tcp://127.0.0.1:61616                      default to openwire
//    tcp://127.0.0.1:61616?wireFormat=openwire  same as above
//    tcp://127.0.0.1:61613?wireFormat=stomp     use stomp instead
//


class MQProducer : public GameMessageProducer
{
private:

   cms::Connection* connection;
   cms::Session* session;
   cms::Destination* m_destination;
   cms::MessageProducer* m_producer;
   int numMessages;
   std::string m_brokerURI;
   std::string m_queueName;

public:

   MQProducer(const std::string& brokerURI,
              const std::string& queueName,
              const std::string& replyName,
              Logger& logger);

   virtual ~MQProducer();

   //virtual void run();

   void sendMessage(const std::string &gameMessage, const std::string& tag);
   bool isSafeToShutdown() const;
private:
   void cleanup();
private:
   Logger& m_Logger;
   const std::string m_replyName;
   const time_t m_shutdownWaitTime;
   time_t m_lastMessageSentOrErrorEventTime;
};


#endif // _MESSAGE_PRODUCER_HPP_
