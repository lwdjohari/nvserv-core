#pragma once

#include <ostream>

#include "nvm/macro.h"
#include "nvserv/global_macro.h"

NVSERV_BEGIN_NAMESPACE(components)

class ComponentLocator;
class ComponentConfig;
class ComponentBase;
class Component;
class ComponentListBase;
class ComponentList;
class ComponentLocator;

using ComponentListBasePtr = std::shared_ptr<ComponentListBase>;
using ComponentListPtr = std::shared_ptr<ComponentList>;
using ComponentBasePtr = std::shared_ptr<ComponentBase>;
using ComponentLocatorPtr = std::shared_ptr<ComponentLocator>;
using ComponentPtr = std::shared_ptr<Component>;
using ComponentConfigPtr = std::shared_ptr<ComponentConfig>;


enum class ComponentType {
  kInknown = 0,
  kServerRest = 1,
  kServerGrpc = 2,
  kServerSocket = 3,
  kCustomDefined = 4,
  kPostgresFeature = 5,
  kOracleFeature = 6,
  kHandlerJson = 7,
  kHandlerBasic = 8,
  kHandlerGrpc = 9,
  kHandlerTcpSocket = 10,
  kHandlerUdpSocket = 11,
  kLogging = 12
};

NVM_ENUM_CLASS_DISPLAY_TRAIT(ComponentType)

NVSERV_END_NAMESPACE
