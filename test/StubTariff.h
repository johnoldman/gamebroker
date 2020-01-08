//
// $Id: StubTariff.h 3484 2007-12-12 11:57:57Z stephend $
//
#include "ITariff.h"

#include <map>

class StubTariff : public ITariff
{
public:
   void addMapping(const std::string& wintype, int startId, int endId);
public:
   virtual std::auto_ptr<AssetClass> getAssetMapping(const WinType& w, const AssetClass& a) const;
   virtual size_t getMappingCount() const;
private:
   class Mapping
   {
   public:
      Mapping(const WinType& w, int ac)
      : m_w(w), m_ac(ac)
      {
      }
      bool operator<(const Mapping& other) const
      {
         if (m_w < other.m_w)
         {
            return true;
         } else if (other.m_w < m_w)
         {
            return false;
         }
         else
         {
            return m_ac < other.m_ac;
         }
      }
   private:
      const WinType m_w;
      const int m_ac;
   };
   std::map<Mapping, int> m_mappings;
};
