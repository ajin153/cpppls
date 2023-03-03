#include "didopen.h"
#include "../../../server/notification/textdocument/publishdiagnostics.h"

void
DidOpenHandler::handle(const nlohmann::json& req_content)
{
    // 1. send "textDocument/PublishDiagnostics" notification
    Handler *handler = new PublishDiagnosticsHandler();
    handler->handle(req_content);
}
