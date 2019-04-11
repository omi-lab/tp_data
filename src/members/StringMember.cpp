#include "tp_data/members/StringMember.h"

namespace tp_data
{
const std::string StringMember::extension{"txt"};

//##################################################################################################
StringMember::StringMember(const std::string& name):
  AbstractMember(name, stringSID())
{

}

//##################################################################################################
StringMember::~StringMember() = default;

//##################################################################################################
StringMember* StringMember::fromData(std::string& error, const std::string& data)
{
  TP_UNUSED(error);
  auto member = new StringMember();
  member->data = data;
  return member;
}

//##################################################################################################
std::string StringMember::toData() const
{
  return data;
}

//##################################################################################################
void StringMember::copyData(const StringMember& other)
{
  data = other.data;
}

}

