#include "tp_data/Globals.h"
#include "tp_data/CollectionFactory.h"
#include "tp_data/members/StringMember.h"

//##################################################################################################
namespace tp_data
{
TDP_DEFINE_ID(                       stringSID,                           "String")

//##################################################################################################
void createCollectionFactories(CollectionFactory& collectionFactory)
{
  collectionFactory.addMemberFactory(new StringMemberFactory());
}

//##################################################################################################
void createAllCollectionFactories(CollectionFactory& collectionFactory)
{
  for(const auto& createCollectionFactories : tp_data::createCollectionFactoriesRegister())
    createCollectionFactories(collectionFactory);
}

//##################################################################################################
std::vector<std::function<void(CollectionFactory&) > > & createCollectionFactoriesRegister()
{
  static std::vector<std::function<void(tp_data::CollectionFactory&)>> createCollectionFactoriesRegister;
  return createCollectionFactoriesRegister;
}

REGISTER_CREATE_COLLECTION_FACTORIES;

//##################################################################################################
int staticInit()
{
  return 0;
}

}
