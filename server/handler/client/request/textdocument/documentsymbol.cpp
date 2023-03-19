#include <future>
#include <regex>
#include <string>

#include "documentsymbol.h"
#include "../../../../parser/uri.h"
#include "../../../../tools/tools.h"

// third-party
#include <boost/process.hpp>
#include <boost/asio.hpp>

/*
{
    "id": 1,
    "jsonrpc": "2.0",
    "method": "textDocument/documentSymbol",
    "params": {
        "textDocument": {
            "uri": "file:///d%3A/Users/User/Desktop/sf_project/z_tmp/cpppls/test/main.pl"
        }
    }
}
 */

void
DocumentSymbolHandler::handle(const nlohmann::json &req_content)
{
    // 1.检查 symbol_scanner 是否可用
    if (!Tools::is_symbol_scanner_enabled()) return;

    // 2.获取文件路径
    std::string raw_uri = req_content["params"]["textDocument"]["uri"];
    URI uri = URI(raw_uri);
    std::string perl_file_path = uri.get_path();

    // 3.获取对应 path 的 file_symbols
    if (!Tools::is_file_symbol_exist(perl_file_path)) return;
    const FileSymbol &file_symbol = Tools::get_file_symbol(perl_file_path);
    const std::vector<SymbolInfo> &symbols = file_symbol.m_symbols;

    std::vector<nlohmann::json> symbol_information_vec;
    for (const auto &symbol : symbols) {
        /* 2023.3.3: 实际行数要-1，vscode的传统艺能 */
        nlohmann::json start {
            { "line", symbol.m_lineno - 1 },
            { "character", symbol.m_length }
        };
        nlohmann::json end {
            { "line", symbol.m_lineno },
            { "character", 0 }
        };
        nlohmann::json range{
            { "start", start },
            { "end", end }
        };
        nlohmann::json location{
            { "uri", raw_uri },
            { "range", range }
        };
        nlohmann::json symbol_information{
            { "name", symbol.m_name },
            { "kind", symbol.m_kind },
            { "location", location }
        };
        symbol_information_vec.push_back(symbol_information);
    }
    nlohmann::json symbol_informations(symbol_information_vec);
    nlohmann::json resp_content{
        { "jsonrpc", "2.0" },
        { "id", req_content["id"] },
        { "result", symbol_informations }
    };

    std::string response = make_message(resp_content);

    send_message(response);

#ifndef NDEBUG
    // -----debug-----
    fmt::print(debug_file, ">>> Sending server response:\n{}\n\n",
               resp_content.dump(4));
    debug_file.flush();
    // ---------------
#endif

}