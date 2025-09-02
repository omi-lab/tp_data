#include "tp_data/Globals.h"
#include "tp_data/CollectionFactory.h"
#include "tp_data/members/StringIDMember.h"
#include "tp_data/members/StringMember.h"
#include "tp_data/members/StringIDVectorMember.h"
#include "tp_data/members/NumberMember.h"

//##################################################################################################
namespace tp_data
{
TP_DEFINE_ID(                       stringSID,                           "String");
TP_DEFINE_ID(                     stringIDSID,                        "String id");
TP_DEFINE_ID(                          intSID,                              "Int");
TP_DEFINE_ID(                        sizeTSID,                           "Size t");
TP_DEFINE_ID(                        floatSID,                            "Float");
TP_DEFINE_ID(                       doubleSID,                           "Double");
TP_DEFINE_ID(               stringIDVectorSID,                 "String id vector");

//##################################################################################################
void createCollectionFactories(CollectionFactory& collectionFactory)
{
  collectionFactory.addMemberFactory(new         StringMemberFactory({52, 192, 235}));
  collectionFactory.addMemberFactory(new       StringIDMemberFactory({25, 144, 181}));
  collectionFactory.addMemberFactory(new StringIDVectorMemberFactory({8, 146, 189}));

  collectionFactory.addMemberFactory(new    IntMemberFactory({212, 99, 193}));
  collectionFactory.addMemberFactory(new  SizeTMemberFactory({166, 63, 148}));
  collectionFactory.addMemberFactory(new  FloatMemberFactory({163, 31, 140}));
  collectionFactory.addMemberFactory(new DoubleMemberFactory({212, 11, 177}));
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
