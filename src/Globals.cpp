#include "tp_data/Globals.h"
#include "tp_data/CollectionFactory.h"
#include "tp_data/members/StringMember.h"

//##################################################################################################
namespace tp_data
{
TDP_DEFINE_ID(                       stringSID,                           "String")

//##################################################################################################
void createCollectionFactories(tp_data::CollectionFactory& collectionFactory)
{
  collectionFactory.addMemberFactory(new StringMemberFactory());
}

}
