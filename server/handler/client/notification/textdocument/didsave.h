#ifndef CPPPLS_TEXTDOCUMENT_DIDSAVE_HANDLER_H
#define CPPPLS_TEXTDOCUMENT_DIDSAVE_HANDLER_H

#include "../../../handler.h"

/*
 * [client notification]
 * method: textDocument/didSave
 */
class DidSaveHandler: public Handler {
public:
    DidSaveHandler() = default;
    ~DidSaveHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif
