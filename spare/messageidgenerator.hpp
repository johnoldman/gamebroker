#ifndef _MESSAGE_ID_GENERATOR_HPP_
#define _MESSAGE_ID_GENERATOR_HPP_

#include <string>
#include <iostream>

// simple little test class for dishing out unique message ids



class MessageIdGenerator
{
private:

	
	// i'm not calling this a singleton!!
	// but for test purposes only one is enough 
	MessageIdGenerator(){};
	~MessageIdGenerator(){};
	MessageIdGenerator(const MessageIdGenerator &i);
	MessageIdGenerator& operator=(const MessageIdGenerator &i);

public:
	static void getMessageId(std::string &messageId);

};

#endif // _MESSAGE_ID_GENERATOR_HPP_

