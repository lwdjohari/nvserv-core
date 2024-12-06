#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "nvserv/global.h"

NVSERV_BEGIN_NAMESPACE(formats::yaml)

class YamlReader;

using YamlReaderPtr = std::shared_ptr<YamlReader>;

class YamlNode{
  public:
    explicit YamlNode(const YamlReaderPtr reader, ryml::NodeRef&& node);
    virtual ~YamlNode();

    const YamlReaderPtr Reader() const;
  private:
    ryml::NodeRef node_;

};

using YamlNodePtr = std::shared_ptr<YamlNode>;

class YamlReader {
 public:
  explicit YamlReader(const std::string& path);
  explicit YamlReader(const uint8_t* bytes, size_t size);
  virtual ~YamlReader();

  const YamlNodePtr Root() const;
  std::vector<YamlNodePtr> Nodes(YamlNodePtr parent) const;
  std::vector<std::string> Keys(YamlNodePtr parent_key) const;
  std::vector<std::string> Keys(const std::string& parent_key) const;

 protected:
  std::shared_ptr<ryml::Tree> tree_;
  ryml::NodeRef root_;

  void LoadFromFile(const std::string& file_path) const;
  void LoadFromBytes(const char* data, std::size_t size) const;

  template <typename StreamType>
  void Parse(StreamType& stream) const;

};

NVSERV_END_NAMESPACE