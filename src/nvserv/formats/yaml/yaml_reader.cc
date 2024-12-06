#include "nvserv/formats/yaml/yaml_reader.h"

NVSERV_BEGIN_NAMESPACE(formats::yaml)


   YamlReader::YamlReader(const std::string& path){

   }

   YamlReader::YamlReader(const uint8_t* bytes, size_t size){


   }

   YamlReader::~YamlReader(){

   }

  const YamlNodePtr YamlReader::Root() const{
    return nullptr;
  }

  std::vector<YamlNodePtr> YamlReader::Nodes(YamlNodePtr parent) const{
    return std::vector<YamlNodePtr>();
  }

  std::vector<std::string> YamlReader::Keys(YamlNodePtr parent_key) const{
     return std::vector<std::string>();
  }
  std::vector<std::string> YamlReader::Keys(const std::string& parent_key) const{
    return std::vector<std::string>();
  }

 // protected:

  void YamlReader::LoadFromFile(const std::string& file_path) const{}
  void YamlReader::LoadFromBytes(const char* data, std::size_t size) const {}

  
NVSERV_END_NAMESPACE