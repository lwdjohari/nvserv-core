#pragma once

#include <memory>
#include <cstdint>
#include <string>

#include "nvserv/components/component.h"
#include "nvserv/global_macro.h"

NVSERV_BEGIN_NAMESPACE(server)

class ServerBase : public components::Component {
 public:
  virtual ~ServerBase(){};

  virtual void Start() = 0;

  virtual void Stop() = 0;

  virtual void Wait() = 0;

  virtual bool IsRun() const = 0;

 protected:
  ServerBase(components::ComponentLocator& locator,
             components::ComponentConfig& config,
             nvserv::components::ComponentType type);
                 
};

NVSERV_END_NAMESPACE