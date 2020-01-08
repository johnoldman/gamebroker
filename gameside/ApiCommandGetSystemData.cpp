//
// $Id: ApiCommandGetSystemData.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "ApiCommandGetSystemData.h"

ApiCommandGetSystemData::ApiCommandGetSystemData(const std::string &messageId,
                                                 const time_t& eventTime,
                          const std::string &systemDataType, 
                          const std::string &dataKey)
    : ApiCommand(messageId, eventTime)
    , systemDataType_(systemDataType)
    , dataKey_(dataKey)
{
}

std::string ApiCommandGetSystemData::getTla() const
{
   return "GSD";
}
