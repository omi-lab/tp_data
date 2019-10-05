#include "tp_data/Globals.h"
#include "tp_data/CollectionFactory.h"
#include "tp_data/members/StringMember.h"
#include "tp_data/members/NumberMember.h"

//##################################################################################################
namespace tp_data
{
tp_qt_DEFINE_ID(                       stringSID,                           "String");
tp_qt_DEFINE_ID(                          intSID,                              "Int");
tp_qt_DEFINE_ID(                        sizeTSID,                           "Size t");
tp_qt_DEFINE_ID(                        floatSID,                            "Float");
tp_qt_DEFINE_ID(                       doubleSID,                           "Double");

//##################################################################################################
void createCollectionFactories(CollectionFactory& collectionFactory)
{
  collectionFactory.addMemberFactory(new StringMemberFactory());

  collectionFactory.addMemberFactory(new    IntMemberFactory());
  collectionFactory.addMemberFactory(new  SizeTMemberFactory());
  collectionFactory.addMemberFactory(new  FloatMemberFactory());
  collectionFactory.addMemberFactory(new DoubleMemberFactory());
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
