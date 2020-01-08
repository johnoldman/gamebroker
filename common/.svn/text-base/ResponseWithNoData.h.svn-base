//
// $Id: ResponseWithNoData.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef RESPONSEWITHNODATA_H_
#define RESPONSEWITHNODATA_H_

#include "ServiceBrokerResponse.h"
#include "SglExceptions.h"

class GameMessageProducer;

class ResponseWithNoData : public ServiceBrokerResponse
{
public:
   explicit ResponseWithNoData(XmlIterator& xmlResponse);
   explicit ResponseWithNoData(const std::string& messageId);
   ResponseWithNoData(const std::string& messageId,               
                      const enum SglGameAPI::SglServerExceptionType& errorCode,
                      const std::string& errorString);
   void send(GameMessageProducer& producer) const;
};

#endif
