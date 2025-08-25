#pragma once

#include "tp_data/Collection.h"
#include "NumberMember.h"

#include <sstream>

namespace tp_data
{

//##################################################################################################
inline float getFloat(const Collection& inputData,
                      const tp_utils::StringID& name,
                      float defaultValue = 0.f)
{
  const FloatMember* m = nullptr;
  inputData.memberCast(name, m);
  return m ? m->data : defaultValue;
}

//##################################################################################################
inline int getInteger(const Collection& inputData,
                      const tp_utils::StringID& name,
                      int defaultValue = 0)
{
  const IntMember* m = nullptr;
  inputData.memberCast(name, m);
  return m ? m->data : defaultValue;
}
}

