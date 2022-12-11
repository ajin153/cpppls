#include "lifecycle.h"
#include "../handler/initialize.h"
#include "../handler/initialized.h"

LifeCycleFactory::LifeCycleFactory(std::string method)
{
    if (method == "initialize") {
        req_type = LifeCycleType::INITIALIZE;
    } else if (method == "initialized") {
        req_type = LifeCycleType::INITIALIZED;
    } else {
        req_type = LifeCycleType::OTHER;
    }
}

RequestHandler* LifeCycleFactory::create_handler()
{
    RequestHandler *req_handler;
    switch(req_type){
    case LifeCycleType::INITIALIZE:
        req_handler = new InitializeHandler();
        break;
    case LifeCycleType::INITIALIZED:
        req_handler = new InitializedHandler();
        break;
    case LifeCycleType::OTHER:
    default:
        req_handler = new RequestHandler();
        break;
    }
    return req_handler;
}