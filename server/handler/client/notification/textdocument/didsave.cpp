#include "didopen.h"
#include "../../../server/notification/textdocument/publishdiagnostics.h"
#include "didsave.h"

void
DidSaveHandler::handle(const nlohmann::json& req_content)
{
    // 1.send "textDocument/PublishDiagnostics" notification
    Handler *handler = new PublishDiagnosticsHandler();
    handler->handle(req_content);
}
