//
// $Id: NotEnoughMoneyException.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include <boost/lexical_cast.hpp>

#include "NotEnoughMoneyException.h"
#include "UserSessionId.h"
#include "GameSessionId.h"

NotEnoughMoneyException::NotEnoughMoneyException(const std::string& context, int purseid)
: std::logic_error(context + " Not Enough Money in Purse Id " + boost::lexical_cast<std::string>(purseid))
{
}
