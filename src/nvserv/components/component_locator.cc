#include "nvserv/components/component_locator.h"

NVSERV_BEGIN_NAMESPACE(components)
ComponentLocator::ComponentLocator(ComponentConfig& configs)
                : components_(nullptr), is_component_list_attached(false) {}

ComponentLocator::~ComponentLocator() {}

void ComponentLocator::AttachComponentList(
    ComponentListBasePtr&& component_list) {
  if (!component_list) {
    DetachComponentList();
    return;
  }

  components_ = std::forward<ComponentListBasePtr>(component_list);
  is_component_list_attached = true;
};

void ComponentLocator::DetachComponentList() {
  is_component_list_attached = false;
  components_ = nullptr;
}

bool ComponentLocator::IsComponentListAttached() const {
  return is_component_list_attached;
}

ComponentListBasePtr ComponentLocator::Components() {
  return components_;
}

const ComponentListBasePtr ComponentLocator::Components() const {
  return components_;
}

const ComponentBasePtr ComponentLocator::Resolve(
    const std::string& name) const {
  if (!components_)
    return nullptr;

  return components_->GetComponent(name);
}

ComponentBasePtr ComponentLocator::Resolve(const std::string& name) {
  if (!components_)
    return nullptr;

  return components_->GetComponent(name);
}

NVSERV_END_NAMESPACE
