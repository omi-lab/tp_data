#ifndef tp_data_Collection_h
#define tp_data_Collection_h

#include "tp_data/Globals.h"

namespace tp_data
{
class AbstractMember;

//##################################################################################################
//! This holds a collection of data objects.
class Collection
{
public:
  //################################################################################################
  Collection();

  //################################################################################################
  ~Collection();

  //################################################################################################
  const std::string& name() const;

  //################################################################################################
  void setName(const std::string& name);

  //################################################################################################
  //! When this data collection was created.
  /*!
  This will be initialized to the current time but factories may change this.
  \return The timestamp of this collection.
  */
  int64_t timestampMS()const;

  //################################################################################################
  //! set the timestamp of this collection.
  void setTimestampMS(int64_t timestampMS);

  //################################################################################################
  void addError(const std::string& error);

  //################################################################################################
  //These are not saved
  const std::vector<std::string>& errors() const;

  //################################################################################################
  void addMember(AbstractMember* member);

  //################################################################################################
  const std::vector<AbstractMember*>& members() const;

  //################################################################################################
  //! Find an member.
  /*!
  \note The Collection owns the returned member.
  \param name The unique name of the member to find.
  \returns A pointer to the member or nullptr.
  */
  AbstractMember* member(const std::string& name) const;

  //################################################################################################
  template<typename T>
  void memberCast(const std::string& name, T*& member_) const
  {
    member_ = dynamic_cast<T*>(member(name));
  }

  //################################################################################################
  void clear();

private:
  struct Private;
  friend struct Private;
  Private* d;
};

}

#endif
