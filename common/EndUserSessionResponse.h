//
// $Id: EndUserSessionResponse.h 2791 2007-11-14 18:24:18Z stephend $
//
#ifndef _END_USER_SESSION_RESPONSE_H_
#define _END_USER_SESSION_RESPONSE_H_

#include <string>

#include "ResponseWithNoData.h"
#include "GameMessageProducer.h"
#include "SglExceptions.h"

class XmlIterator;
class ResponseMessageFactory;

class EndUserSessionResponse : public ResponseWithNoData
{
public:
   explicit EndUserSessionResponse(XmlIterator& xmlResponse);
   explicit EndUserSessionResponse(const std::string& messageId);
   EndUserSessionResponse(const std::string& messageId,               
                     const enum SglGameAPI::SglServerExceptionType& errorCode,
                     const std::string& errorString);
   static std::string getTag();
   std::string getTla() const;
};

#endif
