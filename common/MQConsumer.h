 //
// $Id: MQConsumer.h 3490 2007-12-12 13:03:10Z stephend $
//
#ifndef _MESSAGE_CONSUMER_HPP_
#define _MESSAGE_CONSUMER_HPP_

//#include <activemq/concurrent/Thread.h>
//#include <activemq/concurrent/Runnable.h>
//#include <activemq/concurrent/CountDownLatch.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
//#include <activemq/util/Integer.h>
//#include <activemq/util/Config.h>
//#include <activemq/util/Date.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
//#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <iostream>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread.hpp>

using namespace activemq::core;
using namespace activemq::util;
//using namespace activemq::concurrent;
using namespace cms;

void consumertest();

class ActiveMqConsumerHandler;
class Logger;

#include "GameMessageConsumer.h"
#include "ThreadSafeBool.h"
#include "ThreadSafeQueue.h"

class MQConsumer : public ExceptionListener,
                   public MessageListener,
		   public GameMessageConsumer
{
private:
    //CountDownLatch latch;
    //CountDownLatch doneLatch;
    Connection* connection;
    Session* session;
    Destination* destination;
    MessageConsumer* consumer;
    const std::string m_brokerURI;
public:
    MQConsumer( const std::string& brokerURI,
                const std::string& queueName,
                Logger& logger);

    virtual ~MQConsumer();    

    //void waitUntilReady() ;
    //virtual void stop();

    // Called from the consumer since this class is a registered MessageListener.
    virtual void onMessage( const Message* message );

    // If something bad happens you see it here as this class is also been
    // registered as an ExceptionListener with the connection.
    virtual void onException( const CMSException& ex AMQCPP_UNUSED);

    // Transparent receiver of messages, not tied to ActiveMQ interface.
    void setConsumer(ActiveMqConsumerHandler* m_pConsumer);
    //void commit();

private:
    void rollback(); // Hack as CMS Session::rollback() doesn't actually appear to work...
    void cleanup();
    void startup();  // Start all CMS MQ stuff going.
private:
    // So we run a thread on which will be executed all the callbacks.
    //void runThread();
    std::auto_ptr<boost::thread> m_pThread;
    // This allows us to signal the thread to stop in a nice manner
    ThreadSafeBool m_bStopped;
    Logger& m_Logger;
    // A queue of messages - used to thread-switch as activeMQ doesn't like us committing 
    // the mq read inside onMessage() for some weird reason.
    ThreadSafeQueue<std::pair<std::string,time_t> > m_messageQueue;
    const std::string m_queueName;
    //boost::condition writeAcknowledged_;
    //volatile bool acknowledged_;
    // Queue used to receive acknowledgement of processing.
    ThreadSafeQueue<bool> m_ackQueue;
    // Mutex to protect access to the consumer object by the client and the MQ thread.
    boost::mutex m_mutex;
    ActiveMqConsumerHandler* m_pConsumer;
    // Mutex to protect access to connection, destination, consumer and session
    boost::mutex m_brokerMutex;
};

#endif // _MESSAGE_CONSUMER_HPP_
