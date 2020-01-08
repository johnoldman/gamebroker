#ifndef _TEST_CLIENT_HPP_
#define _TEST_CLIENT_HPP_

#include <string>

class TestClient
{
private:
	const std::string gameSessionId_;
	const std::string userId_;
	const std::string userSessionId_;
	const long gameCost_;
	const long gameFloorLimit_;
	const std::string eventCode_;
	const long amount_; 			//  The amount to debit/credit.
		
	long purseValue;
	
	// prevent reproduction
//	TestClient(const TestClient& l){} 
	TestClient& operator=(const TestClient&){return *this;}

public:
	TestClient(const std::string &gameSessionId,
						const std::string &userId,
						const std::string &userSessionId,
						const long gameCost,
						const long gameFloorLimit);
	~TestClient();
	void shot(TestClient *c);
	void shotBy(TestClient *c);
	void getUserId(std::string &Id);
};

#endif // _TEST_CLIENT_HPP_
