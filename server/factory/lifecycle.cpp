#include "lifecycle.h"
#include "../handler/client/request/initialize.h"
#include "../handler/client/notification/initialized.h"

LifeCycleFactory::LifeCycleFactory(const std::string& method)
{
    if (method == "initialize") {
        req_type = LifeCycleType::INITIALIZE;
    } else if (method == "initialized") {
        req_type = LifeCycleType::INITIALIZED;
    } else {
        req_type = LifeCycleType::OTHER;
    }
}

Handler*
LifeCycleFactory::create_handler()
{
    Handler *req_handler;
    switch(req_type){
    case LifeCycleType::INITIALIZE:
        req_handler = new InitializeHandler();
        break;
    case LifeCycleType::INITIALIZED:
        req_handler = new InitializedHandler();
        break;
    case LifeCycleType::OTHER:
    default:
        req_handler = new Handler();
        break;
    }
    return req_handler;
}
