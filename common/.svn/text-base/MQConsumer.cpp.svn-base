//
// $Id: MQConsumer.cpp 3577 2007-12-13 16:46:08Z stephend $
//
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
#include "MQConsumer.h"
#include "ActiveMqConsumerHandler.h"
#include "Logger.h"

void consumertest()
{

    std::cout << "=====================================================\n";
    std::cout << "Starting the example:" << std::endl;
    std::cout << "-----------------------------------------------------\n";

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
    std::string brokerURI =
        "tcp://127.0.0.1:61616"
        "?wireFormat=openwire"
        "&transport.useAsyncSend=true";
//        "&transport.commandTracingEnabled=true"
//        "&transport.tcpTracingEnabled=true";
//        "&wireFormat.tightEncodingEnabled=true";

    //============================================================
    // set to true to use topics instead of queues
    // Note in the code above that this causes createTopic or
    // createQueue to be used in both consumer an producer.
    //============================================================
    //bool useTopics = false;
    //int numMessages = 10;

    long long startTime = Date::getCurrentTimeMilliseconds();

    //MQConsumer consumer( brokerURI, numMessages, useTopics );

    // Start the consumer thread.
    //Thread consumerThread( &consumer );
    //consumerThread.start();

    // Wait for the consumer to indicate that its ready to go.
    //consumer.waitUntilReady();

    //consumerThread.join();

    long long endTime = Date::getCurrentTimeMilliseconds();
    double totalTime = (endTime - startTime) / 1000.0;

    std::cout << "Time to completion = " << totalTime << " seconds." << std::endl;
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Finished with the example." << std::endl;
    std::cout << "=====================================================\n";
}

MQConsumer::MQConsumer(const std::string& brokerURI,
                       const std::string& queueName,
                       Logger& logger):
//latch(1),
//doneLatch(1),
connection(0),
session(0),
destination(0),
consumer(0)
,m_brokerURI(brokerURI)
, m_bStopped(false)
, m_Logger(logger)
, m_messageQueue(1)
, m_queueName(queueName)
, m_pConsumer(0)
{
   startup();
}

void MQConsumer::startup()
{
   boost::mutex::scoped_lock lock(m_brokerMutex);

   // Create a ConnectionFactory
   ActiveMQConnectionFactory connectionFactory(m_brokerURI);

   // Create a Connection
   connection = connectionFactory.createConnection();
   connection->start();

   connection->setExceptionListener(this);

   // Create a Session
   //session = connection->createSession( Session::AUTO_ACKNOWLEDGE );
   session = connection->createSession(Session::CLIENT_ACKNOWLEDGE);
   // SESSION_TRANSACTED mode appears not to work at all - rollback is broken.
   //session = connection->createSession( Session::SESSION_TRANSACTED );

   // Using exclusive mode appears to screw up the load-balancing so only 1 consumer can be used.
   //destination = session->createQueue(m_queueName + "?consumer.exclusive=true");
   destination = session->createQueue(m_queueName);

   // Create a MessageConsumer from the Session to the Topic or Queue
   consumer = session->createConsumer(destination);
   consumer->setMessageListener(this);
}

// If something bad happens you see it here as this class is also been
// registered as an ExceptionListener with the connection.
void MQConsumer::onException( const CMSException& ex )
{
   bool bDone = false;
   int sleepTime = 1;
   while (!bDone)
   {
      boost::mutex::scoped_lock lock(m_mutex);
      if (m_pConsumer)
      {
         m_pConsumer->error(ex.getMessage());
      }
      lock.unlock();
      // After a connection failure, tear down and start again.
      cleanup();
      try
      {
         startup();
         bDone = true;
      }
      catch (const CMSException& e)
      {
         m_Logger.unexpectedException(e);
      }
      catch (...)
      {
         m_Logger.unknownException("MQConsumer::onException - recovery");
      }
      if (sleepTime < 30)
      {
         ++sleepTime;
      }
      sleep(sleepTime);
   }
}

void MQConsumer::cleanup()
{
   boost::mutex::scoped_lock lock(m_brokerMutex);
   //*************************************************
   // Always close destination, consumers and producers before
   // you destroy their sessions and connection.
   //*************************************************

   // Destroy resources.
   try
   {
      delete destination;
   }
   catch (CMSException& e)
   {
      e.printStackTrace();
   }
   destination = NULL;

   try
   {
      delete consumer;
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }
   consumer = NULL;

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

   // Now Destroy them
   try
   {
      delete session;
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }
   session = NULL;

   try
   {
      delete connection;
   }
   catch(CMSException& e)
   {
      e.printStackTrace();
   }
   connection = NULL;
}

MQConsumer::~MQConsumer()
{
   //m_bStopped.set(true);
   //m_messageQueue.kick();
   cleanup();
   //if (m_pThread.get())
   //{
   //   m_pThread->join();
   //}
}

//void MQConsumer::stop()
//{
//   doneLatch.countDown();
//}
//
//void MQConsumer::waitUntilReady()
//{
//   latch.await();
//}

void MQConsumer::setConsumer(ActiveMqConsumerHandler* pConsumer)
{
   boost::mutex::scoped_lock lock(m_mutex);
   m_pConsumer = pConsumer;
//   if (!m_pThread.get())
//   {
//      m_pThread = std::auto_ptr<boost::thread>(new boost::thread(boost::bind(&MQConsumer::runThread, this)));
//   }
}
//
// This function is meant to do rollbacks, however Apache CMS seems to have a bug which
// means that rollback() doesn't actually roll back. So this function does lots of stuff
// (including a session close) to make the rollback happen.
//
void MQConsumer::rollback()
{
   if(session)
   {
      session->rollback();
      //session->close();
      //session = 0;
   }
}

//void MQConsumer::runThread()
//{
//   while (!m_bStopped)
//   {
//      bool bAck = true;
//      try
//      {
//         std::pair<std::string, time_t> message = m_messageQueue.blockingRead();
//         boost::mutex::scoped_lock lock(m_mutex);
//         if (m_pConsumer)
//         {
//            m_pConsumer->receive(message.first, message.second);
//         }
//         //session->commit();
//  //       message.acknowledge();
//      }
//      catch (const ThreadSafeQueue<std::string>::QueueKickedException&)
//      {
//         // This is the normal stopping condition - so don't log (at least not seriously)
//         // Probably need to rollback though, just in case as we might have read a message.
//         // Rollback doesn't work, so just close the session
//         bAck = false;
//         session->close();
//         session = 0;
//      }
//      catch (const std::exception& exception)
//      {
//         rollback();
//         m_Logger.unexpectedException(exception);
//      }
//      catch (...)
//      {
//         rollback();
//         m_Logger.unknownException("MQConsumer::runThread");
//      }
//     // acknowledge the write through the condition variable + predecate
//      if (bAck)
//      {
//         m_ackQueue.write(true);
//      }
//   }
//}

// Called from the consumer since this class is a registered MessageListener.
void MQConsumer::onMessage(const Message* message)
{
//   static int count = 0;
//   count++;

   const TextMessage* textMessage = dynamic_cast< const TextMessage* >( message );
   if( textMessage != 0 )
   {
      try
      {
         const std::string text = textMessage->getText();
         m_Logger.messageReceived(text);

         boost::mutex::scoped_lock lock(m_mutex);
         if( m_pConsumer )
         {
            m_pConsumer->receive(text, textMessage->getCMSTimestamp() / 1000L);
         }
         message->acknowledge();
      }
      catch (const std::exception& exception)
      {
         rollback();
         m_Logger.unexpectedException(exception);
      }
      catch (...)
      {
         rollback();
         m_Logger.unknownException("MQConsumer::runThread");
      }
   }
   else
   {
      m_Logger.unknownException("MQConsumer::Received something other than a text message");
   }
}
