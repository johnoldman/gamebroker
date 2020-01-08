//
// $Id: RegisterGameInstanceResponse.h 2791 2007-11-14 18:24:18Z stephend $
//
#ifndef _REGISTER_GAME_INSTANCE_RESPONSE_H_
#define _REGISTER_GAME_INSTANCE_RESPONSE_H_

#include "GameMessageProducer.h"
#include "ResponseWithNoData.h"

class ResponseMessageFactory;

class RegisterGameInstanceResponse : public ResponseWithNoData
{
public:
   RegisterGameInstanceResponse(XmlIterator& xmlResponse);
   RegisterGameInstanceResponse(const std::string& messageId);
   RegisterGameInstanceResponse(const std::string& messageId,               
                   const enum SglGameAPI::SglServerExceptionType& errorCode,
                   const std::string& errorString);
   static std::string getTag();
   std::string getTla() const;
};

#endif
