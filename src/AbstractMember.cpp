#include "tp_data/AbstractMember.h"

#include "tp_utils/TimeUtils.h"

namespace tp_data
{

//##################################################################################################
AbstractMember::AbstractMember(const std::string& name, const tp_utils::StringID& type):
  m_name(name),
  m_type(type),
  m_timestampMS(tp_utils::currentTimeMS())
{

}

//##################################################################################################
AbstractMember::~AbstractMember() = default;

//##################################################################################################
const std::string& AbstractMember::name() const
{
  return m_name;
}

//##################################################################################################
void AbstractMember::setName(const std::string& name)
{
  m_name = name;
}

//##################################################################################################
const tp_utils::StringID& AbstractMember::type() const
{
  return m_type;
}

//##################################################################################################
int64_t AbstractMember::timestampMS() const
{
  return m_timestampMS;
}

//##################################################################################################
void AbstractMember::setTimestampMS(int64_t timestampMS)
{
  m_timestampMS = timestampMS;
}

}
