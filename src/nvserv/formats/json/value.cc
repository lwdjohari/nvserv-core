#include "nvserv/formats/json/value.h"

NVSERV_BEGIN_NAMESPACE(formats::json)

Value::Value()
                : allocator_(document_.GetAllocator()),
                  current_value_(&document_),
                  is_empty_(true) {
  document_.SetObject();
}

Value::Value(const Value& other)
                : document_(),
                  allocator_(document_.GetAllocator()),
                  current_value_(&document_),
                  is_empty_(other.is_empty_) {
  document_.CopyFrom(other.document_, allocator_);
}

Value& Value::operator=(const Value& other) {
  if (this != &other) {
    document_.CopyFrom(other.document_, allocator_);
    current_value_ = &document_;
    is_empty_ = other.is_empty_;
  }
  return *this;
}

Value& Value::operator[](const std::string& key) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  if (!document_.HasMember(key.c_str())) {
    rapidjson::Value json_key(key.c_str(), allocator_);
    document_.AddMember(json_key, rapidjson::Value(), allocator_);
  }

  current_value_ = &document_[key.c_str()];
  return *this;
}

Value& Value::operator=(const std::string& value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  if (current_value_) {
    *current_value_ = rapidjson::Value(value.c_str(), allocator_);
  }
  return *this;
}

Value& Value::operator=(const char* value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  if (current_value_) {
    *current_value_ = rapidjson::Value(value, allocator_);
  }
  return *this;
}

Value& Value::operator=(int value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  if (current_value_) {
    *current_value_ = rapidjson::Value(value);
  }

  return *this;
}

Value& Value::operator=(double value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  if (current_value_) {
    *current_value_ = rapidjson::Value(value);
  }

  return *this;
}

Value& Value::operator=(bool value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  if (current_value_) {
    *current_value_ = rapidjson::Value(value);
  }

  return *this;
}

Value& Value::operator=(const std::optional<std::string>& value) {
  if (value.has_value()) {
    *this = value.value();
  } else {
    *current_value_ = rapidjson::Value(rapidjson::kNullType);
  }

  return *this;
}

Value& Value::operator=(const std::optional<int>& value) {
  if (value.has_value()) {
    *this = value.value();
  } else {
    *current_value_ = rapidjson::Value(rapidjson::kNullType);
  }

  return *this;
}

Value& Value::operator=(const std::optional<double>& value) {
  if (value.has_value()) {
    *this = value.value();
  } else {
    *current_value_ = rapidjson::Value(rapidjson::kNullType);
  }

  return *this;
}

Value& Value::operator=(const std::optional<bool>& value) {
  if (value.has_value()) {
    *this = value.value();
  } else {
    *current_value_ = rapidjson::Value(rapidjson::kNullType);
  }

  return *this;
}

Value& Value::operator=(const std::vector<Value>& value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  rapidjson::Value array(rapidjson::kArrayType);
  for (const auto& item : value) {
    rapidjson::Value v(rapidjson::kObjectType);
    v.CopyFrom(item.document_, allocator_);
    array.PushBack(v, allocator_);
  }

  if (current_value_) {
    *current_value_ = array;
  }

  return *this;
}

Value& Value::operator=(const std::vector<std::string>& value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  rapidjson::Value array(rapidjson::kArrayType);
  for (const auto& item : value) {
    array.PushBack(rapidjson::Value(item.c_str(), allocator_), allocator_);
  }

  if (current_value_) {
    *current_value_ = array;
  }

  return *this;
}

Value& Value::operator=(const std::vector<int>& value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  rapidjson::Value array(rapidjson::kArrayType);
  for (int item : value) {
    array.PushBack(item, allocator_);
  }

  if (current_value_) {
    *current_value_ = array;
  }

  return *this;
}

Value& Value::operator=(const std::vector<double>& value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  rapidjson::Value array(rapidjson::kArrayType);
  for (double item : value) {
    array.PushBack(item, allocator_);
  }

  if (current_value_) {
    *current_value_ = array;
  }

  return *this;
}

Value& Value::operator=(const std::vector<bool>& value) {
  if (is_empty_) {
    document_.SetObject();
    is_empty_ = false;
  }

  rapidjson::Value array(rapidjson::kArrayType);
  for (bool item : value) {
    array.PushBack(item, allocator_);
  }

  if (current_value_) {
    *current_value_ = array;
  }

  return *this;
}

std::vector<uint8_t> Value::SerializeToBytes() const {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  if (!document_.Accept(writer)) {
    throw std::runtime_error("Failed to serialize JSON document.");
  }

  const char* json_str = buffer.GetString();
  return __NR_RETURN_MOVE(std::vector<uint8_t>(json_str, json_str + buffer.GetSize()));
}
NVSERV_END_NAMESPACE