#ifndef CPPPLS_TEXTDOCUMENT_PUBLISHDIAGNOSTICS_HANDLER_H
#define CPPPLS_TEXTDOCUMENT_PUBLISHDIAGNOSTICS_HANDLER_H

#include "../../../handler.h"
#include <unordered_map>

/*
 * [server notification]
 * method: textdocument/PublishDiagnostics
 */
class PublishDiagnosticsHandler : public Handler {
public:
    PublishDiagnosticsHandler() = default;
    ~PublishDiagnosticsHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif