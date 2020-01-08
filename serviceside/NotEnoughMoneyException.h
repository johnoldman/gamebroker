//
// $Id: NotEnoughMoneyException.h 3484 2007-12-12 11:57:57Z stephend $
//
#include <stdexcept>

class UserSessionId;
class GameSessionId;

class NotEnoughMoneyException : public std::logic_error
{
public:
   NotEnoughMoneyException(const std::string& context, int purseid);
};
