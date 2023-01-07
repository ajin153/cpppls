#ifndef CPPPLS_TEXTDOCUMENT_PUBLISHDIAGNOSTICS_HANDLER_H
#define CPPPLS_TEXTDOCUMENT_PUBLISHDIAGNOSTICS_HANDLER_H

#include "../../../handler.h"

extern std::string g_perl_exe;
extern std::vector<std::string> g_perl_incs;

// [server notification]
// method: textdocument/PublishDiagnostics
class PublishDiagnosticsHandler : public Handler {
public:
    PublishDiagnosticsHandler() = default;
    ~PublishDiagnosticsHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif
