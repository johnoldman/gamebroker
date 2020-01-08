//
// $Id: PurseNotFoundException.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "PurseNotFoundException.h"
#include "UserSessionId.h"
#include "GameSessionId.h"

PurseNotFoundException::PurseNotFoundException(const std::string& context, const GameSessionId& gamesessionid, const UserSessionId& sessionid)
: std::logic_error(context + " Purse " + sessionid.toString() + " not found in game " + gamesessionid.toString())
{
}
