#include "initialize.h"

void
InitializeHandler::handle(const nlohmann::json &req_content)
{
    // 1.set initialize base config
    nlohmann::json completion_item{
        { "labelDetailsSupport", "true" }
    };
    auto signature_trigger_characters = nlohmann::json::array({"(", ","});
    nlohmann::json signature_help_provider{
        { "triggerCharacters", signature_trigger_characters }
    };
    nlohmann::json textdocument_sync{
        { "openClose", true },
        { "change", 2 },
        { "save", true }
    };
    auto completion_trigger_characters = nlohmann::json::array({
        ">", ":", "$", "@", "%", " ", "-"});
    nlohmann::json completion_provider{
        { "triggerCharacters", completion_trigger_characters },
        { "resolveProvider", true }
    };
    nlohmann::json workspace_folders{
        { "supported", true },
        { "changeNotifications", true }
    };
    nlohmann::json workspace{
        { "workspaceFolders", workspace_folders }
    };
    nlohmann::json result{
        {
            "capabilities",
            {   { "completionItem", completion_item },
                { "definitionProvider", true },
                { "documentSymbolProvider", true },
                { "hoverProvider", false },
                { "signatureHelpProvider", signature_help_provider },
                { "textDocumentSync", textdocument_sync },
                { "documentFormattingProvider", true },
                { "documentRangeFormattingProvider", true },
                { "completionProvider", completion_provider },
                { "workspaceSymbolProvider", true },
                { "workspace", workspace }
            }
        }
    };
    nlohmann::json resp_content{
        { "jsonrpc", "2.0" },
        { "id", req_content["id"] },
        { "result", result }
    };

    // 2.make initialize response
    std::string response = make_message(resp_content);

    // 3.send initialize response
    send_message(response);

    // -----debug-----
    fmt::print(debug_file, ">>> Sending server response:\n{}\n\n",
               resp_content.dump(4));
    // ---------------

}