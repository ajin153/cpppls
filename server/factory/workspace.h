#ifndef CPPPLS_WORKSPACE_FACTORY_H
#define CPPPLS_WORKSPACE_FACTORY_H

#include "request_handler.h"

class WorkspaceFactory : public RequestHandlerFactory {
public:
    WorkspaceFactory() = default;
    virtual ~WorkspaceFactory() = default;

    RequestHandler* create_handler() override;
};

#endif