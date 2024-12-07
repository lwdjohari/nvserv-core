#include "nvserv/server/server_base.h"

NVSERV_BEGIN_NAMESPACE(server)

  ServerBase::ServerBase(components::ComponentLocator& locator,
             components::ComponentConfig& config,
             nvserv::components::ComponentType type)
                  : components::Component(locator, config, type) {}


NVSERV_END_NAMESPACE