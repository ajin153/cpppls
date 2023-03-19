#ifndef CPPPLS_TEXTDOCUMENT_DIDOPEN_HANDLER_H
#define CPPPLS_TEXTDOCUMENT_DIDOPEN_HANDLER_H

#include "../../../handler.h"

/*
 * [client notification]
 * method: textDocument/didOpen
 */
class DidOpenHandler: public Handler {
public:
    DidOpenHandler() = default;
    ~DidOpenHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif
