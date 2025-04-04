#pragma once

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "nvserv/global.h"

NVSERV_BEGIN_NAMESPACE(formats::json)

struct JsonBuffer {
  const char* data;
  size_t size;
  std::shared_ptr<rapidjson::StringBuffer> buffer;

  explicit JsonBuffer() : data(nullptr), size(), buffer(nullptr) {}

  std::string ToString() const {
    if (!buffer)
      return std::string();
    return __NR_RETURN_MOVE(std::string(data, size));
  }
};

struct JsonBuilderNode {
  std::string key;
  rapidjson::Value* value;

  JsonBuilderNode(const std::string& key, rapidjson::Value* value)
                  : key(key), value(value) {}
};

enum class JsonValueType {
  kUnknown = -1,    //!< still undetermined until next =
  kNullType = 0,    //!< null
  kFalseType = 1,   //!< false
  kTrueType = 2,    //!< true
  kObjectType = 3,  //!< object
  kArrayType = 4,   //!< array
  kStringType = 5,  //!< string
  kNumberType = 6   //!< number
};
class ValueBuilder {
 public:
  ValueBuilder();
  ValueBuilder(const ValueBuilder& other);  // Copy constructor
  ValueBuilder& operator=(
      const ValueBuilder& other);  // Copy assignment operator
  ValueBuilder& operator[](const std::string& key);

  ValueBuilder& operator=(const std::string& value);
  ValueBuilder& operator=(const char* value);
  ValueBuilder& operator=(unsigned int value);
  ValueBuilder& operator=(int value);
  ValueBuilder& operator=(float value);
  ValueBuilder& operator=(double value);
  ValueBuilder& operator=(bool value);
  // void operator=(const std::optional<std::string>& value);
  // void operator=(const std::optional<int>& value);
  // void operator=(const std::optional<double>& value);
  // void operator=(const std::optional<bool>& value);
  // void operator=(const std::vector<ValueBuilder>& value);
  // void operator=(const std::vector<std::string>& value);
  // void operator=(const std::vector<int>& value);
  // void operator=(const std::vector<double>& value);
  // void operator=(const std::vector<bool>& value);

  void CreateJsonObject();

  JsonBuffer SerializeToBytes() const;
  JsonBuffer SerializePrettyToBytes() const;

 private:
  rapidjson::Document document_;
  rapidjson::Value* current_value_;
  rapidjson::Document::AllocatorType& allocator_;
  bool is_empty_;
  JsonValueType current_type_;
  std::vector<JsonBuilderNode> value_nodes_;
  rapidjson::Value* GetNodeValue();
};

NVSERV_END_NAMESPACE