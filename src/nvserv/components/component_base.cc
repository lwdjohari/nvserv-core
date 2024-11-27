#include <nvserv/components/component_base.h>

// cppcheck-suppress unknownMacro
NVSERV_BEGIN_NAMESPACE(components)

ComponentBase::ComponentBase(const components::ComponentLocator& locator,
                             const components::ComponentConfig& config,
                             ComponentType type, bool is_monitor)
                : component_type_(type){}



NVSERV_END_NAMESPACE
