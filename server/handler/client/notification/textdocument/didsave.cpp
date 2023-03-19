#include "didsave.h"
#include "../../../../tools/tools.h"
#include "../../../../parser/uri.h"
#include "../../../server/notification/textdocument/publishdiagnostics.h"

/*
{
    "jsonrpc": "2.0",
    "method": "textDocument/didSave",
    "params": {
        "textDocument": {
            "uri": "file:///d%3A/Users/User/Desktop/sf_project/z_tmp/cpppls/test/aaa.pm"
        }
    }
}
*/

void
DidSaveHandler::handle(const nlohmann::json& req_content)
{
    std::string raw_uri = req_content["params"]["textDocument"]["uri"];
    URI uri = URI(raw_uri);
    std::string perl_file_path = uri.get_path();
    // 1.update file content
    Tools::update_file_content(perl_file_path);

    // 2.update file symbol and index
    Tools::update_file_symbol_and_index(perl_file_path);

    // 3.send "textDocument/PublishDiagnostics" notification
    Handler *handler = new PublishDiagnosticsHandler();
    handler->handle(req_content);
}