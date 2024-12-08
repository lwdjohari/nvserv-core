#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <string>
#include <vector>
#include <nvm/macro.h>
#include <nvserv/exceptions.h>
#include "nvserv/global.h"
#include <nvm/types/type_utility.h>
NVSERV_BEGIN_NAMESPACE(formats::yaml)

enum class YamlNodeType{
  Unknown=0,
  Map = 1,
  Sequence = 2,
  Scalar = 3,
  Invalid = 4
};

class YamlReader;

class YamlNode {
 public:
  explicit YamlNode(const YamlReader* reader,
                    std::shared_ptr<ryml::NodeRef> node, bool is_ok);
  virtual ~YamlNode();

  const YamlReader* Reader() const;
  bool Ok() const;
  YamlNodeType Type() const;
 private:
  const YamlReader* reader_;
  std::shared_ptr<ryml::NodeRef> node_;
  bool is_ok_;
  YamlNodeType node_type_;

  YamlNodeType GetNodeType(std::shared_ptr<ryml::NodeRef> node) const;

  friend std::shared_ptr<ryml::NodeRef> __GetNodeRef(const YamlNode& node, const YamlReader* reader);
};

using YamlNodePtr = std::shared_ptr<YamlNode>;

class YamlReader {
 public:
  explicit YamlReader(const std::string& path);
  explicit YamlReader(const uint8_t* bytes, size_t size);
  virtual ~YamlReader();

  YamlNodePtr Root() const;
  std::vector<YamlNodePtr> Nodes(const YamlNodePtr parent) const;
  std::vector<std::string> Keys(const YamlNodePtr parent) const;
  std::vector<std::string> Keys(const std::string& parent_key) const;

  const std::string& Filepath() const;
  bool IsOk() const;

 protected:
  std::shared_ptr<ryml::Tree> tree_;
  std::shared_ptr<ryml::NodeRef> root_;

  void LoadFromFile(const std::string& file_path);
  void LoadFromBytes(const char* data, std::size_t size);

  template <typename TStreamType>
  void Parse(TStreamType& stream);
  

 private:
  std::shared_ptr<std::ifstream> file_handle_;
  std::string file_path_;

  void CheckNodePreOpsThrowsExceptionOnUnmeet(const YamlNodePtr node) const;

  bool IsNodeIsSeqOrMap(const YamlNodePtr parent) const;
  bool IsNodeIsSeqOrdMapOrScalar(const YamlNodePtr parent) const;
};


// Parse any stream based backup by stringstream
// TODO: Dirty implementations on memory, next optimization add option to parsed
// in arena or in place to able to handle large files
template <typename TStreamType>
void YamlReader::Parse(TStreamType& stream) {
  // avoid double buffer if TStreamType is std::stringstream
  if constexpr (nvm::types::utility::is_type_of_v<TStreamType,
                                                  std::stringstream>) {
    std::stringstream& buffer = stream;

    try {
      tree_ = std::make_shared<ryml::Tree>(
          ryml::parse_in_arena(ryml::to_csubstr(buffer.str())));
      root_ = std::make_shared<ryml::NodeRef>(tree_->rootref());
    } catch (const std::exception& e) {
      throw nvm::RuntimeException("Failed to parse YAML: " +
                                  std::string(e.what()));
    }
  } else {
    std::stringstream buffer;
    buffer << stream.rdbuf();

    try {
      tree_ = std::make_shared<ryml::Tree>(
          ryml::parse_in_arena(ryml::to_csubstr(buffer.str())));
      root_ = std::make_shared<ryml::NodeRef>(tree_->rootref());
    } catch (const std::exception& e) {
      throw nvm::RuntimeException("Failed to parse YAML: " +
                                  std::string(e.what()));
    }
  }
}

NVSERV_END_NAMESPACE