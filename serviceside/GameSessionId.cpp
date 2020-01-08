#include "GameSessionId.h"

GameSessionId::GameSessionId(const std::string& id)
: m_id(id)
{
}

void GameSessionId::printOn(std::ostream& os) const
{
   os << m_id;
}

std::string GameSessionId::toString() const
{
   return m_id;
}

std::ostream& operator<<(std::ostream& os, const GameSessionId& gsi)
{
   gsi.printOn(os);
   return os;
}

bool GameSessionId::operator<(const GameSessionId& other) const
{
   return m_id < other.m_id;
}

bool GameSessionId::operator==(const GameSessionId& other) const
{
   return m_id == other.m_id;
}
