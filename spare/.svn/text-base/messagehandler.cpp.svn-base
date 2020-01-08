#include "../common/messageproducer.hpp"
#include "messagehandler.hpp"


MessageHandler::MessageHandler()
{
	try
	{
		std::string brokerURI =
		"tcp://127.0.0.1:61616"
		"?wireFormat=openwire"
		"&transport.useAsyncSend=true";
	//        "&transport.commandTracingEnabled=true"
	//        "&transport.tcpTracingEnabled=true";
	//        "&wireFormat.tightEncodingEnabled=true";

		bool useTopics = false;
		mqproducer = new MQProducer(brokerURI, "Ledger2Purses", useTopics);

		// Start the producer thread.
		Thread producerThread(mqproducer);
		producerThread.start();

		// Wait for the threads to complete.
		producerThread.join();
	}
	catch(...)
	{
		std::cout << "MessageHandler: constructor threw an unknown exception" << std::endl;
	}
	std::cout << "MessageHandler: instanciated" << std::endl;
}

MessageHandler::~MessageHandler()
{
	delete mqproducer;
	std::cout << "MessageHandler: distroyed" << std::endl;
}

void MessageHandler::send(const std::string &message)
{
	try
	{
		if(mqproducer != NULL)
		{
			mqproducer->send(message);
		}
		else
		{
			std::cout<< "oh no, mqproducer is null" << std::endl;
		}
	}
	catch(...)
	{
		std::cout << "MessageHandler: send threw an unknown exception" << std::endl;
	}
}



