//
// $Id: GameSessionId.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef GAME_SESSION_ID_H_
#define GAME_SESSION_ID_H_

#include <string>
#include <iosfwd>

class GameSessionId
{
public:
   explicit GameSessionId(const std::string& id);
   void printOn(std::ostream&) const;
   std::string toString() const;
   bool operator<(const GameSessionId& other) const;
   bool operator==(const GameSessionId& other) const;
private:
   std::string m_id;
};

std::ostream& operator<<(std::ostream&, const GameSessionId& gsi);

#endif
