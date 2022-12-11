#ifndef CPPPLS_TEXTDOCUMENT_FACTORY_H
#define CPPPLS_TEXTDOCUMENT_FACTORY_H

#include "request_handler.h"

class TextDocumentFactory : public RequestHandlerFactory {
public:
    TextDocumentFactory() = default;
    virtual ~TextDocumentFactory() = default;

    RequestHandler* create_handler() override;
};

#endif