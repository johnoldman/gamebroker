#include <string>
#include <iostream>
#include "SkillGameServiceClient.h"
#include "messageidgenerator.hpp"
#include "testclient.hpp"

using namespace SglGameAPI;


TestClient::TestClient(const std::string &gameSessionId,
						const std::string &userId,
						const std::string &userSessionId,
						const long gameCost,
						const long gameFloorLimit):
gameSessionId_(gameSessionId),
userId_(userId),
userSessionId_(userSessionId),
gameCost_(gameCost),
gameFloorLimit_(gameFloorLimit),
eventCode_("deadly"),
amount_(10)
{

	std::string messageId;
	MessageIdGenerator::getMessageId(messageId);	

	try
	{
		purseValue = GameClient::beginUserSession(messageId, gameSessionId_, userId_, userSessionId_, gameCost_, gameFloorLimit_);
	}
	catch(SglCommunicationError &a)
	{
		std::cout << "TestClient: beginUserSession exception: Communication Error" << std::endl;
	}
	catch(SglUnknownGameSessionId &b)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown Game Session ID" << std::endl;
	}
	catch(SglGameClosed &c)
	{
		std::cout << "TestClient: beginUserSession exception: Game Closed" << std::endl;
	}
	catch(SglInsufficiantFunds &e)
	{
		std::cout << "TestClient: beginUserSession exception: Insufficient Funds" << std::endl;
	}
	catch(SglDuplicateUserSessionId &f)
	{
		std::cout << "TestClient: beginUserSession exception: Duplicate User Session ID" << std::endl;
	}
	catch(SglUnknownUserId &d)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown User ID" << std::endl;
	}
	catch(...)
	{
		std::cout << "TestClient: beginUserSession threw an unknown exception" << std::endl;
	}

	std::cout << "TestClient "<< userId_ << " user session id " << userSessionId << " Session started with " << purseValue 
	          << " in the purse"  << std::endl;
}

TestClient::~TestClient()
{
	std::string messageId;
	MessageIdGenerator::getMessageId(messageId);	

	try
	{
		GameClient::endUserSession(messageId, gameSessionId_, userSessionId_);
	}
	catch(SglCommunicationError &e)
	{
		std::cout << "TestClient: beginUserSession exception: Communication Error" << std::endl;
	}
	catch(SglUnknownGameSessionId &e)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown Game Session ID" << std::endl;
	}
	catch(SglGameClosed &e)
	{
		std::cout << "TestClient: beginUserSession exception: Game Closed" << std::endl;
	}
	catch(SglFundsRemaining &e)
	{
		std::cout << "TestClient: beginUserSession exception: Funds Remaining" << std::endl;
	}
	catch(...)
	{
		std::cout << "TestClient: endUserSession threw an unknown exception" << std::endl;
	}
			
	std::cout << userId_ << " distroyed" << std::endl;
}


void TestClient::shot(TestClient *c)
{
	std::string victimId;	
	 
	c->getUserId(victimId); 
	
		
	
	std::cout << userId_ << " shot by " << victimId << std::endl;
	
	std::string messageId;
	MessageIdGenerator::getMessageId(messageId);	

	try
	{
		GameClient::makePayment(messageId, gameSessionId_, victimId, userId_, amount_, eventCode_);
	}
	catch(SglCommunicationError &e)
	{
		std::cout << "TestClient: beginUserSession exception: Communication Error" << std::endl;
	}
	catch(SglUnknownGameSessionId &e)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown Game Session ID" << std::endl;
	}
	catch(SglGameClosed &e)
	{
		std::cout << "TestClient: beginUserSession exception: Game Closed" << std::endl;
	}
	catch(SglUnknownUserSessionId &e)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown User Session ID" << std::endl;
	}
	catch(SglInsufficiantFunds &e)
	{
		std::cout << "TestClient: beginUserSession exception: Insufficient Funds" << std::endl;
	}
	catch(SglUnknownEventCode &e)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown Event Code" << std::endl;
	}
	catch(...)
	{
		std::cout << "TestClient: endUserSession threw an unknown exception" << std::endl;
	}
}





void TestClient::shotBy(TestClient *c)
{
	std::string shooterId;
	
	c->getUserId(shooterId);

	std::string messageId;
	MessageIdGenerator::getMessageId(messageId);	

	try
	{
		GameClient::makePayment(messageId, gameSessionId_, userId_, shooterId, amount_, eventCode_);
	}
	catch(SglCommunicationError &e)
	{
		std::cout << "TestClient: beginUserSession exception: Communication Error" << std::endl;
	}
	catch(SglUnknownGameSessionId &e)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown Game Session ID" << std::endl;
	}
	catch(SglGameClosed &e)
	{
		std::cout << "TestClient: beginUserSession exception: Game Closed" << std::endl;
	}
	catch(SglUnknownUserSessionId &e)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown User Session ID" << std::endl;
	}
	catch(SglInsufficiantFunds &e)
	{
		std::cout << "TestClient: beginUserSession exception: Insufficient Funds" << std::endl;
	}
	catch(SglUnknownEventCode &e)
	{
		std::cout << "TestClient: beginUserSession exception: Unknown Event Code" << std::endl;
	}
	catch(...)
	{
		std::cout << "TestClient: endUserSession threw an unknown exception" << std::endl;
	}


}

void TestClient::getUserId(std::string &Id)
{
	Id = userId_;	
}





