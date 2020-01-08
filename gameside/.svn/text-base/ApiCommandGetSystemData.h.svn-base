//
// $Id: ApiCommandGetSystemData.h 2982 2007-11-21 10:05:17Z stephend $
//
#ifndef API_COMMAND_GET_SYSTEM_DATA_H_
#define API_COMMAND_GET_SYSTEM_DATA_H_

#include "ApiCommand.h"

class ApiCommandGetSystemData: public ApiCommand
{
public:
    ApiCommandGetSystemData(const std::string &messageId,
                            const time_t& eventTime,
                          const std::string &systemDataType, 
                          const std::string &dataKey);
    std::string getTla() const; 
private:
    const std::string systemDataType_;
    const std::string dataKey_;
};

#endif // _API_COMMAND_GET_SYSTEM_DATA_H_

