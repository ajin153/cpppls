#include "textdocument.h"
#include "../handler/client/textdocument/didopen.h"
#include "../handler/client/textdocument/documentsymbol.h"

TextDocumentFactory::TextDocumentFactory(const std::string& method)
{
    if (method == "didOpen") {
        req_type = TextDocumentType::DIDOPEN;
    } else if (method == "documentSymbol") {
        req_type = TextDocumentType::DOCUMENTSYMBOL;
    } else {
        req_type = TextDocumentType::OTHER;
    }
}

Handler*
TextDocumentFactory::create_handler()
{
    Handler *req_handler;
    switch(req_type) {
    case TextDocumentType::DIDOPEN:
        req_handler = new DidOpenHandler();
        break;
    case TextDocumentType::DOCUMENTSYMBOL:
        req_handler = new DocumentSymbolHandler();
        break;
    default:
        req_handler = new Handler();
        break;
    }
    return req_handler;
}