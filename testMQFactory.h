#ifndef _TEST_MQ_FACTORY_H_
#define _TEST_MQ_FACTORY_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class StubLogger;
class MQFactory;

class testMQFactory : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(testMQFactory);
   CPPUNIT_TEST(testConsumerFactory);
   CPPUNIT_TEST(testProducerFactory);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
protected:
   void testConsumerFactory();
   void testProducerFactory();
private:
   StubLogger* logger;
   MQFactory* producerFactory;
   std::string brokerUri;
   std::string subject;
   std::string queueName;
};

#endif
