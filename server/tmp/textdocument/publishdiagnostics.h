#ifndef CPPPLS_TEXTDOCUMENT_PUBLISHDIAGNOSTICS_HANDLER_H
#define CPPPLS_TEXTDOCUMENT_PUBLISHDIAGNOSTICS_HANDLER_H

#include "../request.h"

// method: textdocument/PublishDiagnostics
class PublishDiagnosticsHandler : public RequestHandler {
public:
    PublishDiagnosticsHandler(std::string perl_exe,
                              std::vector<std::string> perl_inc):
        m_perl_exe(perl_exe), m_perl_inc(perl_inc) {}
    std::optional<std::string> handle(const nlohmann::json&) override;
private:
    std::string m_perl_exe; // perl.exe的路径
    std::vector<std::string> m_perl_inc;  // perl支持搜索包的路径(@inc)
};

#endif
