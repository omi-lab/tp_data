#ifndef tp_data_Globals_h
#define tp_data_Globals_h

#include "tp_utils/StringID.h"

//##################################################################################################
//! A pipeline for performing image processing
namespace tp_data
{
class CollectionFactory;

TDP_DECLARE_ID(                       stringSID,                           "String")
TDP_DECLARE_ID(                          intSID,                              "Int")
TDP_DECLARE_ID(                        sizeTSID,                           "Size t")
TDP_DECLARE_ID(                        floatSID,                            "Float")
TDP_DECLARE_ID(                       doubleSID,                           "Double")

//##################################################################################################
//! Add the collection factories that this module provides to the CollectionFactory
void createCollectionFactories(CollectionFactory& collectionFactory);

//##################################################################################################
//! Add all of the collection factories that have been registered to the CollectionFactory
void createAllCollectionFactories(CollectionFactory& collectionFactory);

//##################################################################################################
std::vector<std::function<void(CollectionFactory&)>>& createCollectionFactoriesRegister();

//##################################################################################################
//! Add this namspaces createCollectionFactories method to the global register.
#define REGISTER_CREATE_COLLECTION_FACTORIES \
extern char createCollectionFactories_reg; \
char createCollectionFactories_reg = [] \
{ \
  tp_data::createCollectionFactoriesRegister().push_back(createCollectionFactories); \
  return 0; \
}()

//##################################################################################################
//! Static initialization of this module, see TP_STATIC_INIT in dependencies.pri
int staticInit();

}

#endif
