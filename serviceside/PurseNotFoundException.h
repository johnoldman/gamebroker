//
// $Id: PurseNotFoundException.h 3484 2007-12-12 11:57:57Z stephend $
//
#include <stdexcept>

class UserSessionId;
class GameSessionId;

class PurseNotFoundException : public std::logic_error
{
public:
   PurseNotFoundException(const std::string& context, const GameSessionId& gamesessionid, const UserSessionId& sessionId);
};
