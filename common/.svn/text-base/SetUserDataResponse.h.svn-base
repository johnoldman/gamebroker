//
// $Id: SetUserDataResponse.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef SET_USER_DATA_H_
#define SET_USER_DATA_H_

#include "ResponseWithNoData.h"

class SetUserDataResponse : public ResponseWithNoData
{
public:
   explicit SetUserDataResponse(XmlIterator& xmlResponse);
   explicit SetUserDataResponse(const std::string& messageId);

   SetUserDataResponse(const std::string& messageId,               
            const enum SglGameAPI::SglServerExceptionType& errorCode,
            const std::string& errorString);
   static std::string getTag();
   std::string getTla() const;
};

#endif
