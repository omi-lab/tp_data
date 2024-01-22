#include "tp_data/members/StringIDVectorMember.h"

#include "tp_utils/JSONUtils.h"

namespace tp_data
{
const std::string StringIDVectorMember::extension{"json"};

//##################################################################################################
StringIDVectorMember::StringIDVectorMember(const std::string& name, const std::vector<tp_utils::StringID>& data_):
  AbstractMember(name, stringSID())
{
  data = data_;
}

//##################################################################################################
StringIDVectorMember::~StringIDVectorMember() = default;

//##################################################################################################
StringIDVectorMember* StringIDVectorMember::fromData(std::string& error, const std::string& data)
{
  TP_UNUSED(error);
  auto member = new StringIDVectorMember();
  tp_utils::getJSONStringIDs(tp_utils::jsonFromString(data), member->data);
  return member;
}

//##################################################################################################
std::string StringIDVectorMember::toData() const
{
  nlohmann::json j;
  tp_utils::saveVectorOfStringIDsToJSON(j, data);
  return j.dump(2);
}

//##################################################################################################
void StringIDVectorMember::copyData(const StringIDVectorMember& other)
{
  data = other.data;
}

}

