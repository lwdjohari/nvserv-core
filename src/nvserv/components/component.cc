#include "nvserv/components/component.h"

NVSERV_BEGIN_NAMESPACE(components)

Component::Component(ComponentType type)
                  : ComponentBase( type) {}
  
Component::Component(const components::ComponentLocator& locator,
                     const components::ComponentConfig& config,
                     ComponentType type)
                  : ComponentBase(locator, config, type, true) {}
  
Component::~Component() {}


NVSERV_END_NAMESPACE
