//
// $Id: MQProducer.cpp 3609 2007-12-14 10:41:17Z stephend $
//
#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/Runnable.h>
//#include <activemq/concurrent/CountDownLatch.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
//#include <activemq/util/Integer.h>
//#include <activemq/util/Config.h>
//#include <activemq/util/Date.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
//#include <cms/BytesMessage.h>
//#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
//#include <iostream>
#include "MQProducer.h"
#include "Logger.h"
#include <time.h>

using namespace cms;

MQProducer::MQProducer( const std::string& brokerURI,
                        const std::string& queueName,
                        const std::string& replyName,
                        Logger& logger)
: connection(NULL)
, session(NULL)
, m_destination(NULL)
, m_producer(NULL)
, m_brokerURI(brokerURI)
, m_queueName(queueName)
, m_Logger(logger)
, m_replyName(replyName)
, m_shutdownWaitTime(300L)
, m_lastMessageSentOrErrorEventTime(time(0))
{
   // Create a ConnectionFactory
   ActiveMQConnectionFactory connectionFactory(m_brokerURI);

   // Create a Connection
   connection = connectionFactory.createConnection();
   connection->start();

   // Create a Session
   //session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
   session = connection->createSession(Session::CLIENT_ACKNOWLEDGE);

   // Create the destination (Topic or Queue)
   m_destination = session->createQueue(m_queueName);
   
   // Create a MessageProducer from the Session to the Topic or Queue
   m_producer = session->createProducer(m_destination);

   m_producer->setDeliveryMode(DeliveryMode::PERSISTENT);
}

void MQProducer::sendMessage(const std::string &gameMessage, const std::string& tag)
{
   // reset time stamp
   m_lastMessageSentOrErrorEventTime = time(0); 
   std::auto_ptr<TextMessage> message(session->createTextMessage(gameMessage));
   message->setStringProperty("JMSXGroupID", tag);
   message->setStringProperty("JMSReplyTo", m_replyName + tag);
   m_Logger.messageSent(gameMessage);
   bool bSent = false;
   // Loop trying to send message
   for (size_t i = 0; i < 5; ++i)
   {
      try 
      {
         m_producer->send(message.get());
         bSent = true;

         break;
      }
      catch (const cms::CMSException& e)
      {
         // reset time stamp
         m_lastMessageSentOrErrorEventTime = time(0); 

         m_Logger.unexpectedException(e);

         delete m_destination;
         m_destination = 0;

         delete m_producer;
         m_producer = 0;

         session->close();
         delete session;
         session = 0;
 
         connection->close();
         delete connection;
         connection = 0;

         // Create a ConnectionFactory
         ActiveMQConnectionFactory connectionFactory(m_brokerURI);

         // Create a Connection
         connection = connectionFactory.createConnection();
         connection->start();

         session = connection->createSession(Session::CLIENT_ACKNOWLEDGE);
         m_destination = session->createQueue(m_queueName);
         m_producer = session->createProducer(m_destination);
         m_producer->setDeliveryMode(DeliveryMode::PERSISTENT);
      }
   }
   // Have one more go - throw exception if it doesn't work.
   if (!bSent)
   {
      m_producer->send(message.get());
   }
}

bool MQProducer::isSafeToShutdown() const
{
   // return true if time from last message send exceeds m_shutdownWaitTime and no reported errors
   // note time values in seconds
   return m_shutdownWaitTime > difftime(time(0), m_lastMessageSentOrErrorEventTime);
}

MQProducer::~MQProducer()
{
   // Destroy resources.
   try
   {
      if( m_destination != NULL )
      {
         delete m_destination;
      }
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }
   m_destination = NULL;

   try
   {
      if( m_producer != NULL )
      {
         delete m_producer;
      }
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }
   m_producer = NULL;

   // Close open resources.
   try
   {
      if(session != NULL)
      {
         session->close();
      }
      if(connection != NULL)
      {
         connection->close();
      }
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }

   try
   {
      if( session != NULL )
      {
         delete session;
      }
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }
   session = NULL;

   try
   {
      if( connection != NULL )
      {
         delete connection;
      }
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }
   connection = NULL;
}
