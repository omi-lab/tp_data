#ifndef tp_data_StringIDVectorMember_h
#define tp_data_StringIDVectorMember_h

#include "tp_data/AbstractMemberFactory.h"

#include "json.hpp"

namespace tp_data
{

//##################################################################################################
class StringIDVectorMember : public tp_data::AbstractMember
{
public:
  //################################################################################################
  StringIDVectorMember(const std::string& name=std::string(), const std::vector<tp_utils::StringID>& data_=std::vector<tp_utils::StringID>());

  //################################################################################################
  ~StringIDVectorMember();

  //################################################################################################
  static StringIDVectorMember* fromData(std::string& error, const std::string& data);

  //################################################################################################
  std::string toData() const;

  //################################################################################################
  void copyData(const StringIDVectorMember& other);

  static const std::string extension;
  std::vector<tp_utils::StringID> data;
};

//##################################################################################################
using StringIDVectorMemberFactory = tp_data::MultiDataMemberFactoryTemplate<StringIDVectorMember, stringIDVectorSID>;

}

#endif
