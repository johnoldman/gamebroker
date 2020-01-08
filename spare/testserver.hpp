#ifndef _TEST_SERVER_HPP_
#define _TEST_SERVER_HPP_

#include "testclient.hpp"
#include <vector>

class TestServer
{
private:
	const int numOfClients_;
	
	// local copies
	const std::string gameName_; 		// The name of the game (e.g. "METTLE")
	const std::string gameType_;       	// The type of game to be played (e.g. "Tournament")
	const std::string gameSessionId_;   // A unique game session id, used by the game server calls to identify which game session is currently in use.
	const long gameCost_;               // The cost of this game.
	const long gameFloorLimit_;	        // The total amount (including game cost), that must be withdrawn from the users Wallet and put into the purse.
	
	// client storage
	typedef std::vector<TestClient*> Container;
	typedef Container::iterator Iter;	
	Container clientStore;
	
	// prevent reproduction
//	TestServer(const TestServer& l){} 
	TestServer& operator=(const TestServer&){return *this;}

public:
	TestServer(const int numOfClients);
	~TestServer();
	void run();
};

#endif // _TEST_SERVER_HPP_
