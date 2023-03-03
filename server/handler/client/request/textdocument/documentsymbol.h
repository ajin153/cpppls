#ifndef CPPPLS_TEXTDOCUMENT_DOCUMENTSYMBOLS_H
#define CPPPLS_TEXTDOCUMENT_DOCUMENTSYMBOLS_H

#include "../../../handler.h"

extern std::string g_perl_exe;

/*
 * [client request]
 * method: textDocument/documentSymbol
 */
class DocumentSymbolHandler: public Handler {
public:
    DocumentSymbolHandler() = default;
    ~DocumentSymbolHandler() = default;
    void handle(const nlohmann::json&);
};

#endif
