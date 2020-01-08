//
// $Id: GameData.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef GAMEDATA_H_
#define GAMEDATA_H_

class GameData
{
public:
   GameData(const std::string& name, const std::string& type, const std::string& tariff) : m_name(name), m_type(type), m_tariff(tariff)
   {
   }
   std::string getName() const { return m_name; }
   std::string getType() const { return m_type; }
   std::string getTariff() const { return m_tariff; }
private:
   std::string m_name;
   std::string m_type;
   std::string m_tariff;
};

#endif
