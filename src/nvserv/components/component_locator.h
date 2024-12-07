#pragma once
#include <memory>

#include "nvserv/components/component_list_base.h"
#include "nvserv/components/declare.h"
#include "nvserv/global_macro.h"

NVSERV_BEGIN_NAMESPACE(components)

class ComponentLocator {
 public:
  explicit ComponentLocator(ComponentConfig& configs);

  virtual ~ComponentLocator();

  void AttachComponentList(ComponentListBasePtr&& component_list) ;

  void DetachComponentList();

  bool IsComponentListAttached() const;

  ComponentListBasePtr Components();

  const ComponentListBasePtr Components() const;

  const ComponentBasePtr Resolve(const std::string& name) const;

  ComponentBasePtr Resolve(const std::string& name);

 private:
  std::shared_ptr<ComponentListBase> components_;
  bool is_component_list_attached;
};

NVSERV_END_NAMESPACE
