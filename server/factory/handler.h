#ifndef CPPPLS_REQUEST_HANDLER_FACTORY_H
#define CPPPLS_REQUEST_HANDLER_FACTORY_H

#include "../handler/handler.h"

// @brief: 只能负责生成在server.cpp中读取到的client request的handler
class HandlerFactory {
public:
    HandlerFactory() = default;
    virtual ~HandlerFactory() = default;

    virtual Handler* create_handler()
    {
        return new Handler();
    }
};

#endif
