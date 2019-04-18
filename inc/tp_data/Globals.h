#ifndef tp_data_Globals_h
#define tp_data_Globals_h

#include "tp_utils/StringID.h"

//##################################################################################################
//! A pipeline for performing image processing
namespace tp_data
{
class CollectionFactory;

TDP_DECLARE_ID(                       stringSID,                           "String")

//##################################################################################################
//! Add the collection factories that this module provides to the CollectionFactory
void createCollectionFactories(tp_data::CollectionFactory& collectionFactory);

}

#endif
