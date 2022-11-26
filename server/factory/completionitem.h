#ifndef CPPPLS_COMPLETIONITEM_FACTORY_H
#define CPPPLS_COMPLETIONITEM_FACTORY_H

#include "request_handler.h"

class CompletionItemFactory : public RequestHandlerFactory {
public:
    CompletionItemFactory() = default;
    virtual ~CompletionItemFactory() = default;

    RequestHandler* create_handler() override;
};

#endif