#include "tp_data/CollectionFactory.h"
#include "tp_data/AbstractMember.h"
#include "tp_data/AbstractMemberFactory.h"
#include "tp_data/Collection.h"

#include "tp_utils/DebugUtils.h"
#include "tp_utils/FileUtils.h"
#include "tp_utils/JSONUtils.h"
#include "tp_utils/StackTrace.h"

#include "json.hpp"

#include <memory>

#ifdef TP_FILESYSTEM
#include <experimental/filesystem>
#endif

namespace tp_data
{

namespace
{

//##################################################################################################
void addPart(std::string& output, const std::string& key, const std::string& data)
{
  uint8_t keyLen = uint8_t(key.size());
  auto partLen = keyLen + data.size() + 5;

  output.reserve(output.size()+partLen);
  output.push_back(static_cast<char>(keyLen));
  output.append(key);

  output.push_back(static_cast<char>(data.size() >>  0));
  output.push_back(static_cast<char>(data.size() >>  8));
  output.push_back(static_cast<char>(data.size() >> 16));
  output.push_back(static_cast<char>(data.size() >> 24));

  output.append(data);
}

//##################################################################################################
bool parsePart(std::string& error, const std::string& input, int& startFrom, std::string& key, std::string& data)
{
  auto ok = [&input, &startFrom, &error](int count)
  {
    bool ok = ((startFrom+count)<=int(input.size()));
    if(!ok)
      error = "Unexpectedly reached end of buffer.";
    return ok;
  };

  //- Read a single unsigned byte that is the key length -------------------------------------------
  if(int(input.size())<=startFrom)
    return false;

  uint8_t keyLen = static_cast<uint8_t>(input.at(size_t(startFrom)));
  startFrom++;


  //- Read the key ---------------------------------------------------------------------------------
  if(!ok(keyLen))
    return false;

  key.resize(keyLen);
  if(keyLen>0)
  {
    memcpy(&key[0], input.data()+startFrom, keyLen);
    startFrom += int(keyLen);
  }


  //- Read a 4 byte unsigned little endian number that is the length of the data -------------------
  if(!ok(4))
    return false;

  uint32_t dataLen = 0;
  for(uint32_t i=0; i<4; i++)
  {
    uint8_t p = static_cast<uint8_t>(input.at(size_t(startFrom)));
    startFrom++;
    dataLen |= (uint32_t(p) << (8*i));
  }


  //- Read the data --------------------------------------------------------------------------------
  if(!ok(int(dataLen)))
    return false;

  data.resize(dataLen);
  if(dataLen>0)
  {
    memcpy(&data[0], input.data()+startFrom, dataLen);
    startFrom += int(dataLen);
  }

  return true;
}

}

//##################################################################################################
struct CollectionFactory::Private
{
  std::unordered_map<tp_utils::StringID, std::unique_ptr<AbstractMemberFactory>> memberFactories;
  bool finalized{false};
};

//##################################################################################################
CollectionFactory::CollectionFactory():
  d(new Private())
{

}

//##################################################################################################
CollectionFactory::~CollectionFactory()
{
  delete d;
}

//##################################################################################################
bool CollectionFactory::finalized() const
{
  return d->finalized;
}

//##################################################################################################
void CollectionFactory::finalize()
{
  if(d->finalized)
  {
    tpWarning() << "Error: CollectionFactory already finalized!";
    tp_utils::printStackTrace();
    return;
  }

  d->finalized = true;
}

//##################################################################################################
void CollectionFactory::addMemberFactory(AbstractMemberFactory* memberFactory)
{
  if(d->finalized)
  {
    tpWarning() << "Error: You can't add member factories to a CollectionFactory that has already been finalized!";
    tp_utils::printStackTrace();
    return;
  }

  d->memberFactories[memberFactory->type()].reset(memberFactory);
}


//##################################################################################################
const AbstractMemberFactory* CollectionFactory::memberFactory(const tp_utils::StringID& type) const
{
  auto i = d->memberFactories.find(type);
  return (i != d->memberFactories.end())?(i->second.get()):nullptr;
}

//##################################################################################################
void CollectionFactory::cloneAppend(std::string& error,
                                    const Collection& collection,
                                    Collection& output,
                                    const std::vector<std::string>& subset) const
{
  for(const auto member : collection.members())
  {
    if(!subset.empty() && !tpContains(subset, member->name()))
      continue;

    const tp_utils::StringID& type = member->type();

    auto factory=memberFactory(type);
    if(!factory)
    {
      error = "Failed to find factory for member type: " + type.keyString();
      continue;
    }

    auto newMember = factory->clone(error, *member);
    if(!newMember)
    {
      error = "Failed to clone member of type: " + type.keyString();
      continue;
    }

    output.addMember(newMember);
  }
}

//##################################################################################################
void CollectionFactory::loadFromData(std::string& error,
                                     const std::string& data,
                                     Collection& output,
                                     const std::vector<std::string>& subset) const
{
  if(data.empty())
  {
    error = "Data is empty.";
    return;
  }

  bool headerSet=false;

  //These hold the details of the current member that we are parsing, once complete addMember()
  //will be called to add the member to the collection.
  std::string currentMemberName;
  int64_t currentMemberTimestamp{0};
  std::string currentMemberType;
  std::string currentMemberData;

  auto addMember = [&, this]()
  {
    if(currentMemberType.empty())
      return true;

    if(!subset.empty() && !tpContains(subset, currentMemberName))
      return true;

    headerSet = true;

    auto factory=memberFactory(currentMemberType);

    if(!factory)
    {
      tpWarning() << "Failed to find member factory for: " << currentMemberType;
      error = "Failed to find member factory for: " + currentMemberType;
      return false;
    }

    auto member = factory->load(error, currentMemberData);

    if(!member || !error.empty())
    {
      tpWarning() << "Valid: " << (member!=nullptr);
      tpWarning() << "Error: " << error;

      tpWarning() << "Failed to load a member, name: " << currentMemberName << " type: " << currentMemberType;
      error = "Failed to load a member, name: " + currentMemberName + " type: " + currentMemberType;
      return false;
    }

    member->setName(currentMemberName);
    member->setTimestampMS(currentMemberTimestamp);
    output.addMember(member);

    currentMemberType.clear();
    currentMemberName.clear();
    currentMemberTimestamp = 0;
    currentMemberData.clear();
    return true;
  };

  auto flushState = [&]()
  {
    return addMember();
  };

  int startFrom = 0;
  std::string key;
  std::string partData;
  while(parsePart(error, data, startFrom, key, partData))
  {
    if(key == "member")
    {
      if(!flushState())
      {
        error = "Flush state error.";
        return;
      }

      currentMemberName = partData;
    }

    else if(key == "type")
    {
      if(!currentMemberName.empty())
        currentMemberType = partData;
    }

    else if(key == "timestamp")
    {
      if(!currentMemberName.empty())
        currentMemberTimestamp = std::stoll(partData);
      else if(!headerSet)
        output.setTimestampMS(std::stoll(partData));
      else
        tpWarning() << "Unexpected timestamp.";
    }

    else if(key == "data")
    {
      if(!currentMemberName.empty())
        currentMemberData = partData;
    }

    else if(key == "name")
    {
      if(!headerSet)
        output.setName(partData);
    }
  }

  if(!flushState())
  {
    error = "Final flush state error.";
    return;
  }

  return;
}

//##################################################################################################
void CollectionFactory::loadFromPath(std::string& error,
                                     const std::string& path,
                                     Collection& output,
                                     const std::vector<std::string>& subset) const
{
  nlohmann::json j = tp_utils::readJSONFile(path + "/index.json");

  output.setName(tp_utils::getJSONValue<std::string>(j, "name"));
  output.setTimestampMS(tp_utils::getJSONValue<int64_t>(j, "timestamp"));

  for(const nlohmann::json& i : tp_utils::getJSONArray(j, "members"))
  {
    auto name = tp_utils::getJSONValue<std::string>(i, "name");
    if(!subset.empty() && !tpContains(subset, name))
      continue;

    auto fileName  = tp_utils::getJSONValue<std::string>(i, "fileName");
    auto type      = tp_utils::getJSONValue<std::string>(i, "type");
    auto timestamp = tp_utils::getJSONValue<int64_t>    (i, "timestamp");

    if(type.empty())
    {
      error = "Empty member type!";
      return;
    }

    auto factory = memberFactory(type);

    if(!factory)
    {
      tpWarning() << "Failed to find member factory for: " << type;
      error = "Failed to find member factory for: " + type;
      return;
    }

    std::string memberPath = path + "/" + fileName;
    auto member = factory->load(error, tp_utils::readBinaryFile(memberPath));


    if(!member || !error.empty())
    {
      tpWarning() << "Valid: " << (member!=nullptr);
      tpWarning() << "Error: " << error;

      tpWarning() << "Failed to load a member, name: " << name << " type: " << type;
      tpWarning() << "  -- From path: " << memberPath;
      error = "Failed to load a member, name: " + name + " type: " + type;
      return;
    }

    member->setName(name);
    member->setTimestampMS(timestamp);
    output.addMember(member);
  }
}

//##################################################################################################
void CollectionFactory::saveToData(std::string& error, const Collection& collection, std::string& data) const
{
  addPart(data, "name", collection.name());
  addPart(data, "timestamp", std::to_string(collection.timestampMS()));

  for(const auto member : collection.members())
  {
    const tp_utils::StringID& type = member->type();

    auto factory=memberFactory(type);

    if(!factory)
    {
      error = "Failed to find factory for member type: " + type.keyString();
      return;
    }

    std::string timestamp = std::to_string(member->timestampMS());
    std::string memberData;
    factory->save(error, *member, memberData);

    if(!error.empty())
    {
      error += "Failed to serialize name:" + member->name() + " type:" + type.keyString();
      return;
    }

    addPart(data, "member", member->name());
    addPart(data, "type", type.keyString());
    addPart(data, "timestamp", timestamp);
    addPart(data, "data", memberData);
  }
}

//##################################################################################################
void CollectionFactory::saveToPath(std::string& error,
                                   const Collection& collection,
                                   const std::string& path,
                                   bool append) const
{
#ifndef TP_FILESYSTEM
  tpWarning() << "CollectionFactory::saveToPath Not supported on this platform!";
  TP_UNUSED(error);
  TP_UNUSED(collection);
  TP_UNUSED(path);
#else
  nlohmann::json membersIndex = nlohmann::json::array();
  nlohmann::json existingMembersIndex = nlohmann::json::array();

  //-- Create the output directory -----------------------------------------------------------------
  std::experimental::filesystem::path dir(path);
  if(std::experimental::filesystem::exists(dir))
  {
    if(!append)
    {
      error = "Output directory already exists.";
      return;
    }

    nlohmann::json j = tp_utils::readJSONFile(path + "/index.json");
    if(j.count("members") == 0)
    {
      error = "Failed to read existing index.";
      return;
    }

    existingMembersIndex = j["members"];
    if(!existingMembersIndex.is_array())
    {
      error = "Existing index is not an array.";
      return;
    }
  }

  else if(!std::experimental::filesystem::create_directory(dir))
  {
    error = "Failed to create output directory.";
    return;
  }

  //-- Save each member to its own file ------------------------------------------------------------
  std::vector<std::string> newMembers;
  newMembers.reserve(collection.members().size());
  for(const auto& member : collection.members())
  {
    const std::string&        name = member->name();
    const tp_utils::StringID& type = member->type();

    auto factory=memberFactory(type);
    if(!factory)
    {
      error = "Failed to find factory for member type: " + type.keyString();
      return;
    }

    std::string data;
    factory->save(error, *member, data);

    if(!error.empty())
    {
      error += "Failed to serialize name:" + name + " type:" + type.keyString();
      return;
    }

    std::string fileName = name + "." + factory->extension();
    std::string filePath = path + "/" + fileName;

    tp_utils::writeBinaryFile(filePath, data);

    {
      newMembers.push_back(name);
      nlohmann::json j;
      j["name"] = name;
      j["fileName"] = fileName;
      j["type"] = type.keyString();
      membersIndex.push_back(j);
    }
  }

  //-- Add in existing members ---------------------------------------------------------------------
  for(const nlohmann::json& i : existingMembersIndex)
    if(auto name = TPJSONString(i, "name"); !name.empty() && !tpContains(newMembers, name))
      membersIndex.push_back(i);

  //-- Create the index file -----------------------------------------------------------------------
  {
    nlohmann::json j;

    j["name"] = collection.name();
    j["timestamp"] = collection.timestampMS();
    j["members"] = membersIndex;
    tp_utils::writePrettyJSONFile(path + "/index.json.new", j);
    //Move would be better.
    tp_utils::copyFile(path + "/index.json.new", path + "/index.json");
  }
#endif
}

//##################################################################################################
AbstractMember* CollectionFactory::clone(std::string& error, const AbstractMember& member) const
{
  auto factory = memberFactory(member.type());
  if(!factory)
  {
    error = "Failed to find factory for member type: " + member.type().keyString();
    return nullptr;
  }

  return factory->clone(error, member);
}

//##################################################################################################
void CollectionFactory::serializeMember(std::string& error,
                                        const AbstractMember& member,
                                        std::string& data,
                                        std::string& extension) const
{
  auto factory = memberFactory(member.type());
  if(!factory)
  {
    error = "Failed to find factory for member type: " + member.type().keyString();
    return;
  }

  factory->save(error, member, data);
  extension = factory->extension();
}

}

