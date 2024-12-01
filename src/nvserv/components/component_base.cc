#include <nvserv/components/component_base.h>

// cppcheck-suppress unknownMacro
NVSERV_BEGIN_NAMESPACE(components)

ComponentBase::ComponentBase(ComponentType type)
                : component_type_(type), is_component_based_(true) {}

ComponentBase::ComponentBase(const components::ComponentLocator& locator,
                             const components::ComponentConfig& config,
                             ComponentType type, bool is_monitor)
                : component_type_(type), is_component_based_(true) {}

NVSERV_END_NAMESPACE
