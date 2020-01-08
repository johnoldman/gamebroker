#include <string>
#include <iostream>
#include "testclient.hpp"
#include "SkillGameServiceClient.h"
#include "messageidgenerator.hpp"
#include "testserver.hpp"


using namespace SglGameAPI;


TestServer::TestServer(const int numOfClients):
numOfClients_(numOfClients),
gameName_("Mettle"),
gameType_("Tournament"),
gameSessionId_("sessionID"),
gameCost_(10000),
gameFloorLimit_(100),
clientStore(Container(100))
{
	std::cout << "TestGameServer: instanciated for running " << numOfClients_ << " clients" << std::endl;
	
	// initialise the Game API
	GameClient::initialise();

	std::string messageId;
	MessageIdGenerator::getMessageId(messageId);	

	// register a game instance	
	try
	{
		GameClient::registerGameInstance(messageId, gameName_,gameType_,gameSessionId_);
	}
	catch(SglCommunicationError &e)
	{
		// Generic communication error
	}
	catch(SglGameSessionTypeNameMatchError &f)
	{
		// A gameSessionId was used with a different game name and type
	}
	catch(...)
	{
		std::cout << "TestGameServer: registerGameInstance threw an unknown exception" << std::endl;
	}

	clientStore.resize(numOfClients_);
	
	char temp1[100], temp2[100];
	// build some of the test clients
	for(int count=0; count<numOfClients_; count++)
	{
		sprintf(temp1, "userId_%d", count);
		std::string userId_ = temp1;
		
		sprintf(temp2, "%s_sessionId_%d", temp1, (count+3));
		std::string userSessionId_ = temp2;	
		
		clientStore[count]=new TestClient(gameSessionId_, userId_, userSessionId_,gameCost_,gameFloorLimit_);
	}
}


TestServer::~TestServer()
{
	for(int count=0; count<numOfClients_; count++)
	{
		if(clientStore[count] != NULL)
		{	
			delete clientStore[count];
		}
	}

	std::string messageId;
	MessageIdGenerator::getMessageId(messageId);	

	// deregister a game instance	
	try
	{
		GameClient::deregisterGameInstance(messageId, gameSessionId_);
	}
	catch(SglCommunicationError &e)
	{
		// Generic communication error
	}
	catch(SglGameSessionTypeNameMatchError &f)
	{
		// A gameSessionId was used with a different game name and type
	}
	catch(...)
	{
		std::cout << "TestGameServer: registerGameInstance threw an unknown exception" << std::endl;
	}
	std::cout << "TestGameServer: distroyed" << std::endl;
}


void TestServer::run()
{
	std::cout << "TestGameServer: running" << std::endl;
	
	for(int count=0; count<numOfClients_; count++)
	{
		if(clientStore[count] != NULL)
		{	
			clientStore[count]->shot(clientStore[3]);
		}
	}
	
	
}





	








