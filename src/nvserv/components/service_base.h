#pragma once

#include <memory>
#include "nvserv/global_macro.h"

NVSERV_BEGIN_NAMESPACE(components)


class ServiceManager;
using ServiceManagerPtr = std::shared_ptr<ServiceManager>;


class ServiceManager
{
private:
    
public:
    ServiceManager();
    ~ServiceManager();
};

class ServiceComponentBase{

};


template <typename TComponent>
class Service{

};



NVSERV_END_NAMESPACE