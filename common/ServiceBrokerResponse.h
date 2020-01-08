//
// $Id: ServiceBrokerResponse.h 2791 2007-11-14 18:24:18Z stephend $
//
#ifndef SERVICE_BROKER_RESPONSE_H_
#define SERVICE_BROKER_RESPONSE_H_

class XmlIterator;

#include <string>
#include "MQMessage.h"

#include "SglExceptions.h"

class ServiceBrokerResponse : public MQMessage
{
public:
   explicit ServiceBrokerResponse(XmlIterator& xmlIterator);
   explicit ServiceBrokerResponse(const std::string& messageId);
   ServiceBrokerResponse(const std::string& messageId,               
                                  const enum SglGameAPI::SglServerExceptionType& errorCode,
                                  const std::string& errorString);
   virtual ~ServiceBrokerResponse() {}
   bool isSuccess() const;
   enum SglGameAPI::SglServerExceptionType getError() const;
   std::string getErrorString() const;
   static std::pair<std::string, XmlIterator> parse(const std::string& message);
protected:
   std::string getSerialisedTagCode() const;
   std::string getSerialisedErrorData() const;
   std::string serialiseResponse() const;
   std::string serialiseResponse(std::string serialisedUserData) const;
private:
   const enum SglGameAPI::SglServerExceptionType m_error;
   std::string m_errorString;
   std::string serialisedErrorData;
};

#endif
