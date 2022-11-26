#include "initialize.h"
#include <optional>

#include <fstream>
#define FMT_HEADER_ONLY
#include "../fmt/format.h"
#include "../fmt/ostream.h"


std::optional<std::string> InitializeHandler::handle(const nlohmann::json &req_content)
{
    nlohmann::json text_document_sync{
        { "openClose", true },
        { "change", 2 },
        { "willSave", false },
        { "willSaveWaitUntil", false },
        { "save", { { "includeText", false } } },
    };
    nlohmann::json completion_provider{
        { "resolveProvider", false },
        { "triggerCharacters", {} },
    };
    nlohmann::json signature_help_provider{
        { "triggerCharacters", "" }
    };
    nlohmann::json code_lens_provider{
        { "resolveProvider", false }
    };
    nlohmann::json document_on_type_formatting_provider{
        { "firstTriggerCharacter", "" },
        { "moreTriggerCharacter", "" },
    };
    nlohmann::json document_link_provider{
        { "resolveProvider", false }
    };
    nlohmann::json execute_command_provider{
        { "commands", {} }
    };
    nlohmann::json result{
        {
            "capabilities",
            {
                { "textDocumentSync", text_document_sync },
                { "hoverProvider", true },
                { "completionProvider", completion_provider },
                { "signatureHelpProvider", signature_help_provider },
                { "definitionProvider", false },
                { "referencesProvider", false },
                { "documentHighlightProvider", false },
                { "documentSymbolProvider", false },
                { "workspaceSymbolProvider", false },
                { "codeActionProvider", false },
                { "codeLensProvider", code_lens_provider },
                { "documentFormattingProvider", false },
                { "documentRangeFormattingProvider", false },
                { "documentOnTypeFormattingProvider", document_on_type_formatting_provider },
                { "renameProvider", false },
                { "documentLinkProvider", document_link_provider },
                { "executeCommandProvider", execute_command_provider },
                { "experimental", {} },
            }
        }
    };
    nlohmann::json resp_content{
        { "jsonrpc", "2.0" },
        { "id", req_content["id"] },
        { "result", result }
    };

    // -----debug-----
    m_resp_content = resp_content;
    // ---------------

    return make_response(resp_content);
}