#ifndef CPPPLS_REQUEST_HANDLER_FACTORY_H
#define CPPPLS_REQUEST_HANDLER_FACTORY_H

#include "../request_handler/handler.h"

class RequestHandlerFactory {
public:
    RequestHandlerFactory() = default;
    virtual ~RequestHandlerFactory() = default;

    virtual RequestHandler* create_handler()
    {
        return new RequestHandler();
    }
};

#endif
