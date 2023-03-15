#ifndef CPPPLS_TEXTDOCUMENT_FACTORY_H
#define CPPPLS_TEXTDOCUMENT_FACTORY_H

#include "handler.h"

// @brief: textdocument/xxx
class TextDocumentFactory: public HandlerFactory {
public:
    enum class TextDocumentType : int {
        DIDOPEN = 0,
        DIDSAVE,
        DOCUMENTSYMBOL,
        DEFINITION,
        OTHER
    };

    TextDocumentFactory(const std::string&);
    ~TextDocumentFactory() = default;

    Handler* create_handler() override;

private:
    TextDocumentType req_type;
};

#endif