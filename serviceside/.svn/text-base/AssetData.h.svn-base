//
// $Id: AssetData.h 3484 2007-12-12 11:57:57Z stephend $
//
#include <string>

#include "Amount.h"

class AssetData
{
public:
   AssetData(const Amount& amount, const std::string& assetClass, int id);
   AssetData(const Amount& amount, const std::string& assetClass, int id, int fromid);
   Amount getAmount() const;
   std::string getAssetClass() const;
   int getId() const;
   int getFromId() const;
private:
   Amount m_amount;
   std::string m_assetClass;
   int m_id;
   int m_fromid;
};
