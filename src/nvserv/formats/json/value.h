#pragma once


#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>

#include "nvserv/global.h"

NVSERV_BEGIN_NAMESPACE (formats::json)

class Value {
 public:
  Value();
  Value(const Value& other);             // Copy constructor
  Value& operator=(const Value& other);  // Copy assignment operator
  Value& operator[](const std::string& key);

  void operator=(const std::string& value);
  void operator=(const char* value);
  void operator=(int value);
  void operator=(double value);
  void operator=(bool value);
  void operator=(const std::optional<std::string>& value);
  void operator=(const std::optional<int>& value);
  void operator=(const std::optional<double>& value);
  void operator=(const std::optional<bool>& value);
  void operator=(const std::vector<Value>& value);
  void operator=(const std::vector<std::string>& value);
  void operator=(const std::vector<int>& value);
  void operator=(const std::vector<double>& value);
  void operator=(const std::vector<bool>& value);

  template <typename T>
  T As() const;

  std::vector<uint8_t> SerializeToBytes() const;

 private:
  rapidjson::Document document_;
  rapidjson::Value* current_value_;
  rapidjson::Document::AllocatorType& allocator_;
  bool is_empty_;

  void Init();
};


template <typename T>
T Value::As() const {
  if (!current_value_) {
    throw std::runtime_error("Null JSON value");
  }

  if constexpr (std::is_same_v<T, std::string>) {
    if (!current_value_->IsString()) {
      throw std::runtime_error("JSON value is not a string");
    }
    return std::string(current_value_->GetString());
  } else if constexpr (std::is_same_v<T, int>) {
    if (!current_value_->IsInt()) {
      throw std::runtime_error("JSON value is not an integer");
    }
    return current_value_->GetInt();
  } else if constexpr (std::is_same_v<T, double>) {
    if (!current_value_->IsDouble()) {
      throw std::runtime_error("JSON value is not a double");
    }
    return current_value_->GetDouble();
  } else if constexpr (std::is_same_v<T, bool>) {
    if (!current_value_->IsBool()) {
      throw std::runtime_error("JSON value is not a boolean");
    }
    return current_value_->GetBool();
  } else if constexpr (std::is_same_v<T, std::optional<std::string>>) {
    if (current_value_->IsNull()) {
      return std::nullopt;
    }
    return As<std::string>();
  } else if constexpr (std::is_same_v<T, std::optional<int>>) {
    if (current_value_->IsNull()) {
      return std::nullopt;
    }
    return As<int>();
  } else if constexpr (std::is_same_v<T, std::optional<double>>) {
    if (current_value_->IsNull()) {
      return std::nullopt;
    }
    return As<double>();
  } else if constexpr (std::is_same_v<T, std::optional<bool>>) {
    if (current_value_->IsNull()) {
      return std::nullopt;
    }
    return As<bool>();
  } else if constexpr (std::is_same_v<T, std::vector<Value>>) {
    if (!current_value_->IsArray()) {
      throw std::runtime_error("JSON value is not an array");
    }
    std::vector<Value> result;
    for (auto& v : current_value_->GetArray()) {
      Value item;
      item.document_.CopyFrom(v, item.allocator_);
      result.push_back(item);
    }
    return result;
  } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
    if (!current_value_->IsArray()) {
      throw std::runtime_error("JSON value is not an array");
    }
    std::vector<std::string> result;
    for (auto& v : current_value_->GetArray()) {
      if (!v.IsString()) {
        throw std::runtime_error("JSON array element is not a string");
      }
      result.push_back(v.GetString());
    }
    return result;
  } else if constexpr (std::is_same_v<T, std::vector<int>>) {
    if (!current_value_->IsArray()) {
      throw std::runtime_error("JSON value is not an array");
    }
    std::vector<int> result;
    for (auto& v : current_value_->GetArray()) {
      if (!v.IsInt()) {
        throw std::runtime_error("JSON array element is not an integer");
      }
      result.push_back(v.GetInt());
    }
    return result;
  } else if constexpr (std::is_same_v<T, std::vector<double>>) {
    if (!current_value_->IsArray()) {
      throw std::runtime_error("JSON value is not an array");
    }
    std::vector<double> result;
    for (auto& v : current_value_->GetArray()) {
      if (!v.IsDouble()) {
        throw std::runtime_error("JSON array element is not a double");
      }
      result.push_back(v.GetDouble());
    }
    return result;
  } else if constexpr (std::is_same_v<T, std::vector<bool>>) {
    if (!current_value_->IsArray()) {
      throw std::runtime_error("JSON value is not an array");
    }
    std::vector<bool> result;
    for (auto& v : current_value_->GetArray()) {
      if (!v.IsBool()) {
        throw std::runtime_error("JSON array element is not a boolean");
      }
      result.push_back(v.GetBool());
    }
    return result;
  } else {
    throw std::runtime_error("Unsupported type for JSON deserialization");
  }
}

NVSERV_END_NAMESPACE