//
// $Id: GetUserDataResponse.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef GET_USER_DATA_H_
#define GET_USER_DATA_H_

#include "GameMessageProducer.h"
#include "ServiceBrokerResponse.h"

class GetUserDataResponse : public ServiceBrokerResponse
{
public:
   explicit GetUserDataResponse(XmlIterator& xmlResponse);
   GetUserDataResponse(const std::string& messageId,  
                       const std::string& userData);
   GetUserDataResponse(const std::string& messageId, 
             const enum SglGameAPI::SglServerExceptionType& errorCode,
             const std::string& errorString);
   std::string getUserData() const { return m_data; }
   static std::string getTag();
   void send(GameMessageProducer& producer) const;
   std::string getTla() const;
private:
   std::string m_data;
   std::string serialisedUserData;
};

#endif
