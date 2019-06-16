#ifndef tp_data_AbstractMemberFactory_h
#define tp_data_AbstractMemberFactory_h

#include "tp_data/AbstractMember.h"

#include "json.hpp"

namespace tp_data
{

//##################################################################################################
//! Used to load / save AbstractMember objects.
/*!
This should be subclassed to save different AbstractMember types.
*/
class AbstractMemberFactory
{
public:
  //################################################################################################
  //! Pass in the type of AbstractMember that this factory works with.
  /*!
  \param type The type of member this factory works with.
  \param extension The file extension to save these members with.
  */
  AbstractMemberFactory(tp_utils::StringID type, std::string extension);

  //################################################################################################
  virtual ~AbstractMemberFactory();

  //################################################################################################
  //! The type of member this factory works with.
  /*!
  The member type is used to select a factory with a matching type.
  \returns The type of member that this factory works with.
  */
  const tp_utils::StringID& type() const;

  //################################################################################################
  //! The file extension for this type of member.
  /*!
  When this type of member is written to file it should be given this extension.
  \return The file extension for this type of member.
  */
  const std::string& extension() const;

  //################################################################################################
  //! Make a clone of the member.
  /*!
  Subclasses should reimplement this to make a clone of members.

  \param error This will be set on error.
  \param member The member to clone.
  \return A clone of the member of nullptr.
  */
  virtual AbstractMember* clone(std::string& error, const AbstractMember& member) const=0;

  //################################################################################################
  //! Save the member to data.
  /*!
  Subclasses should reimplement this to save members.

  \param error This will be set on error.
  \param member The member to clone.
  \param data The output data.
  */
  virtual void save(std::string& error, const AbstractMember& member, std::string& data) const=0;

  //################################################################################################
  virtual AbstractMember* load(std::string& error, const std::string& data) const=0;

private:
  const tp_utils::StringID m_type;
  std::string m_extension;
};

//##################################################################################################
template<typename T, const tp_utils::StringID&(*type_)()>
class JSONMemberFactoryTemplate : public AbstractMemberFactory
{
public:
  //################################################################################################
  JSONMemberFactoryTemplate():
    AbstractMemberFactory(type_(), "json")
  {

  }

  //################################################################################################
  AbstractMember* clone(std::string& error, const AbstractMember& member) const override
  {
    auto m = dynamic_cast<const T*>(&member);
    if(!m)
    {
      error = "Failed to find member of type " + type().keyString();
      return nullptr;
    }

    return makeMember<T>("", m->data);
  }

  //################################################################################################
  void save(std::string& error, const AbstractMember& member, std::string& data) const override
  {
    auto m = dynamic_cast<const T*>(&member);
    if(!m)
    {
      error = "Failed to find member of type " + type().keyString();
      return;
    }

    data = m->toJSON().dump();
  }

  //################################################################################################
  AbstractMember* load(std::string& error, const std::string& data) const override
  {
    try
    {
      auto j = nlohmann::json::parse(data);
      return T::fromJSON(j);
    }
    catch(...)
    {
      error = "Failed to parse JSON for " + type().keyString();
      return nullptr;
    }
  }
};

//##################################################################################################
template<typename T, const tp_utils::StringID&(*type_)()>
class MultiDataMemberFactoryTemplate : public AbstractMemberFactory
{
public:
  //################################################################################################
  MultiDataMemberFactoryTemplate():
    AbstractMemberFactory(type_(), T::extension)
  {

  }

  //################################################################################################
  AbstractMember* clone(std::string& error, const AbstractMember& member) const override
  {
    auto m = dynamic_cast<const T*>(&member);
    if(!m)
    {
      error = "Failed to find member of type " + type().keyString();
      return nullptr;
    }

    auto newMember = new T();
    newMember->copyData(*m);
    return newMember;
  }

  //################################################################################################
  void save(std::string& error, const AbstractMember& member, std::string& data) const override
  {
    auto m = dynamic_cast<const T*>(&member);
    if(!m)
    {
      error = "Failed to find member of type " + type().keyString();
      return;
    }

    data = m->toData();
  }

  //################################################################################################
  AbstractMember* load(std::string& error, const std::string& data) const override
  {
    return T::fromData(error, data);
  }
};

}

#endif
