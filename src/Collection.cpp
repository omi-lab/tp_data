#include "tp_data/Collection.h"
#include "tp_data/AbstractMember.h"

#include "tp_utils/TimeUtils.h"

namespace tp_data
{

//##################################################################################################
struct Collection::Private
{
  TP_NONCOPYABLE(Private);
  std::string name;
  int64_t timestampMS{tp_utils::currentTimeMS()};
  std::vector<std::string> errors;
  std::vector<AbstractMember*> members;

  //################################################################################################
  Private()=default;

  //################################################################################################
  ~Private()
  {
    deleteAll();
  }

  //################################################################################################
  void deleteAll()
  {
    errors.clear();

    tpDeleteAll(members);
    members.clear();
  }
};

//##################################################################################################
Collection::Collection():
  d(new Private())
{

}

//##################################################################################################
Collection::~Collection()
{
  delete d;
}

//##################################################################################################
const std::string& Collection::name() const
{
  return d->name;
}

//##################################################################################################
void Collection::setName(const std::string& name)
{
  d->name = name;
}

//##################################################################################################
int64_t Collection::timestampMS()const
{
  return d->timestampMS;
}

//##################################################################################################
void Collection::setTimestampMS(int64_t timestampMS)
{
  d->timestampMS = timestampMS;
}

//##################################################################################################
void Collection::addError(const std::string& error)
{
  d->errors.push_back(error);
}

//##################################################################################################
const std::vector<std::string>& Collection::errors() const
{
  return d->errors;
}

//##################################################################################################
void Collection::addMember(AbstractMember* member)
{
  d->members.push_back(member);
}

//##################################################################################################
const std::vector<AbstractMember*>& Collection::members() const
{
  return d->members;
}

//##################################################################################################
AbstractMember* Collection::member(const tp_utils::StringID& name) const
{
  for(auto member : d->members)
    if(member->name() == name)
      return member;

  return nullptr;
}

//##################################################################################################
void Collection::clear()
{
  d->deleteAll();
}

}
