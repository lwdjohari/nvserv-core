#include "nvserv/formats/yaml/yaml_reader.h"

#include <nvm/types/type_utility.h>
#include <nvserv/exceptions.h>

#include <sstream>

NVSERV_BEGIN_NAMESPACE(formats::yaml)

YamlReader::YamlReader(const std::string& path) {
  LoadFromFile(path);
}

YamlReader::YamlReader(const uint8_t* bytes, size_t size) {
  if (!bytes) {
    nvserv::NullReferenceException("Yaml bytes null reference exception");
  }
}

YamlReader::~YamlReader() {
  if (!file_handle_ && file_handle_->is_open()) {
    file_handle_->close();
  }
}

bool YamlReader::IsOk() const {
  if (!root_) {
    return false;
  }

  return true;
}

YamlNodePtr YamlReader::Root() const {
  if (!root_) {
    return __NR_RETURN_MOVE(std::make_shared<YamlNode>(this, nullptr, false));
  }

  return __NR_RETURN_MOVE(std::make_shared<YamlNode>(this, root_, true));
}

void YamlReader::CheckNodePreOpsThrowsExceptionOnUnmeet(
    const YamlNodePtr node) const {
  if (!node) {
    throw nvserv::NullReferenceException("ParentNode null-reference");
  }

  if (!node->Ok()) {
    throw nvserv::RuntimeException(
        "ParentNode not in Ok status, can not do operations");
  }
}

std::vector<YamlNodePtr> YamlReader::Nodes(const YamlNodePtr parent) const {
  std::vector<YamlNodePtr> result;
  CheckNodePreOpsThrowsExceptionOnUnmeet(parent);

  std::shared_ptr<ryml::NodeRef> node_ref = __GetNodeRef(*parent, this);

  if (!node_ref) {
    return result;
  }

  if (!IsNodeIsSeqOrMap(parent)) {
    return result;
  }

  for (ryml::NodeRef child = node_ref->first_child(); !child.invalid();
       child = child.next_sibling()) {
    result.push_back(std::move(std::make_shared<YamlNode>(
        this, std::move(std::make_shared<ryml::NodeRef>(child)),
        true)));
  }

  return __NR_RETURN_MOVE(result);
}

std::vector<std::string> YamlReader::Keys(const YamlNodePtr parent) const {
  CheckNodePreOpsThrowsExceptionOnUnmeet(parent);

  std::shared_ptr<ryml::NodeRef> node_ref = __GetNodeRef(*parent, this);

  if (!node_ref) {
    return std::vector<std::string>();
  }

  return std::vector<std::string>();
}

std::vector<std::string> YamlReader::Keys(const std::string& parent_key) const {
  return std::vector<std::string>();
}

const std::string& YamlReader::Filepath() const {
  return file_path_;
};

// protected:

void YamlReader::LoadFromFile(const std::string& file_path) {
  file_handle_ = std::make_shared<std::ifstream>(
      file_path, std::ios::in | std::ios::binary);

  if (!file_handle_->is_open()) {
    nvserv::IoException("Failed to open yaml file on: " + file_path);
  }

  file_path_ = std::string(file_path);
  Parse(*file_handle_);
}

void YamlReader::LoadFromBytes(const char* data, std::size_t size) {
  if (!data || size == 0) {
    throw nvm::InvalidArgException(
        "Failed to load Yaml byte, data is empty or null.");
  }

  std::stringstream stream;
  stream.write(data, static_cast<std::streamsize>(size));
  Parse(stream);
}



bool YamlReader::IsNodeIsSeqOrMap(const YamlNodePtr parent) const {
  auto node_ref = __GetNodeRef(*parent, this);
  if (!node_ref){
    return false;
  }

  if (!node_ref->is_map() && !node_ref->is_seq()) {
    return false;
  }

  return true;
}

// YamlNode Impl

YamlNode::YamlNode(const YamlReader* reader,
                   std::shared_ptr<ryml::NodeRef> node, bool is_ok)
                : reader_(reader),
                  node_(node),
                  is_ok_(is_ok),
                  node_type_(GetNodeType(node)) {}

YamlNode::~YamlNode() {}

const YamlReader* YamlNode::Reader() const {
  return reader_;
}

bool YamlNode::Ok() const {
  return is_ok_;
}

YamlNodeType YamlNode::Type() const {
  return node_type_;
}

YamlNodeType YamlNode::GetNodeType(std::shared_ptr<ryml::NodeRef> node) const {
  if (!node){
    return YamlNodeType::Invalid;
  }

  if (node->is_map()) {
    return YamlNodeType::Map;
  } else if (node->is_seq()) {
    return YamlNodeType::Sequence;
  } else if (node->is_val()) {
    return YamlNodeType::Scalar;
  } else if (node->invalid()) {
    return YamlNodeType::Invalid;
  } else {
    return YamlNodeType::Unknown;
  }
}

std::shared_ptr<ryml::NodeRef> __GetNodeRef(const YamlNode& node,
                                            const YamlReader* reader) {
  if (!reader) {
    throw nvserv::NullReferenceException(
        "YamlReader null reference while trying to __GetNodeRef to YamlNode");
  }

  if (reader != node.reader_) {
    throw nvserv::RuntimeException(
        "YamlNode is not own by YamlReader, can not "
        "passing node that don't owned by YamlReader");
  }

  return node.node_;
}

NVSERV_END_NAMESPACE