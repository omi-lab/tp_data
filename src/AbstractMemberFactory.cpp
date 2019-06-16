#include "tp_data/AbstractMemberFactory.h"

namespace tp_data
{

//##################################################################################################
AbstractMemberFactory::AbstractMemberFactory(tp_utils::StringID type, std::string extension):
  m_type(std::move(type)),
  m_extension(std::move(extension))
{

}

//##################################################################################################
AbstractMemberFactory::~AbstractMemberFactory() = default;

//##################################################################################################
const tp_utils::StringID& AbstractMemberFactory::type() const
{
  return m_type;
}

//##################################################################################################
const std::string& AbstractMemberFactory::extension() const
{
  return m_extension;
}


}
