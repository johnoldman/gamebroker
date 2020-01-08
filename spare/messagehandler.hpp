#ifndef _MESSAGEHANDLER_HPP_
#define _MESSAGEHANDLER_HPP_

#include "../common/messageproducer.hpp"



// handler for message producer

class MQProducer;


class MessageHandler
{
private:
	MQProducer *mqproducer;

	// prevent unplanned reproduction
//	MessageHandler(const MessageHandler& l){} 
//	MessageHandler& operator=(const MessageHandler&){return *this;}

public:
	MessageHandler();
	~MessageHandler();
	void send(const std::string &message);
};




#endif // _MESSAGEHANDLER_HPP_
