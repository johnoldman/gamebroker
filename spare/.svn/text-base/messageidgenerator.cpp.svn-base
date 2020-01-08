#include <string>
#include <iostream>
#include "messageidgenerator.hpp"

int id = 0;	

void MessageIdGenerator::getMessageId(std::string &messageId)
{
	id++;
	char idName[22];
	sprintf(idName, "G-%d", id);
	messageId = idName;	
}
