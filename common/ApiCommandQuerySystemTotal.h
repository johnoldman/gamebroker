//
// $Id: ApiCommandQuerySystemTotal.h 3003 2007-11-21 16:29:13Z stephend $
//
#ifndef _API_COMMAND_QUERY_SYSTEM_TOTAL_H_
#define _API_COMMAND_QUERY_SYSTEM_TOTAL_H_

#include "ApiCommand.h"

class ApiCommandQuerySystemTotal: public ApiCommand
{
public:
    ApiCommandQuerySystemTotal(const std::string &messageId, 
                               const time_t& eventTime,
                               const std::string &gameSessionId,
                               const std::string &totalType);
    std::string getTla() const;
private:
    const std::string gameSessionId_;
    const std::string totalType_; 
};


#endif // _API_COMMAND_QUERY_SYSTEM_TOTAL_H_

