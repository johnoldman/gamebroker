//
// $Id: ApiCommandQuerySystemTotal.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "ApiCommandQuerySystemTotal.h"

ApiCommandQuerySystemTotal::ApiCommandQuerySystemTotal(const std::string &messageId, 
                                                       const time_t& eventTime,
                               const std::string &gameSessionId,
                               const std::string &totalType)
    : ApiCommand(messageId, eventTime)
    , gameSessionId_(gameSessionId)
    , totalType_(totalType)
{
}

std::string ApiCommandQuerySystemTotal::getTla() const
{
   return "QST";
}
