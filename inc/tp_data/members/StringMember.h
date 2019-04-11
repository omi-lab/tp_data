#ifndef tp_data_StringMember_h
#define tp_data_StringMember_h

#include "tp_data/AbstractMemberFactory.h"

#include "json.hpp"

namespace tp_data
{

//##################################################################################################
class StringMember : public tp_data::AbstractMember
{
public:
  //################################################################################################
  StringMember(const std::string& name=std::string());

  //################################################################################################
  ~StringMember();

  //################################################################################################
  static StringMember* fromData(std::string& error, const std::string& data);

  //################################################################################################
  std::string toData() const;

  //################################################################################################
  void copyData(const StringMember& other);

  static const std::string extension;
  std::string data;
};

//##################################################################################################
using StringMemberFactory = tp_data::MultiDataMemberFactoryTemplate<StringMember, stringSID>;

}

#endif
