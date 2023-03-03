#include <future>

#include "documentsymbol.h"
#include "../../../parser/uri.h"

// third-party
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <regex>
#include <string>

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
    // 1.获取文件路径
    std::string raw_uri = req_content["params"]["textDocument"]["uri"];
    URI uri = URI(raw_uri);
    std::string perl_file_path = uri.get_path();
    /*
     * 2.使用"perl_scanner"扫描出文件内的所有Package和Function，目前仅支持这两种
     * ---2023.3.2
     */
#ifdef _WIN32
    std::string perl_scan_cmd = "d:/Users/User/Desktop/sf_project/z_tmp/cpppls/server/tools/perl_scanner/perl_scanner.exe";
#elif __linux__
    std::string perl_scan_cmd = "/home/ajin/Desktop/ajpj/cpppls/server/tools/perl_scanner/perl_scanner";
#endif
    std::future<std::string> result;
    boost::asio::io_service ios;
    boost::process::child perl_scanner_child(perl_scan_cmd,
        perl_file_path,
        boost::process::std_in.close(),
        boost::process::std_out > result,
        boost::process::std_err > boost::process::null,
        ios);
    ios.run();
    std::string perl_scanner_data = result.get();
    size_t pos = 0;
#ifdef _WIN32
    std::string delimiter = "\r\n";
#elif __linux__
    std::string delimiter = "\n";
#endif
    std::vector<nlohmann::json> symbol_information_vec;
    while ((pos = perl_scanner_data.find(delimiter)) != std::string::npos) {
        std::string line = perl_scanner_data.substr(0, pos);
        perl_scanner_data.erase(0, pos + delimiter.length());
        /*
         * line-example: TEST::AJIN,4,1,17
         * 说明：
         * 每一行中包含对应lsp协议中SymbolInformation类型所需要的数据
         * 分别为: name,kind,lineno,linelength
         * 其中lineno和linelength对应Range类型的start和character(Location->Range)
         */
        std::regex line_delimiter(",");
        std::vector<std::string> strs(std::sregex_token_iterator(line.begin(),
            line.end(), line_delimiter, -1), std::sregex_token_iterator());
        std::string name = strs[0];
        int kind = std::stoi(strs[1]);
        int lineno = std::stoi(strs[2]);
        int linelength = std::stoi(strs[3]);
        /* 2023.3.3: 实际行数要-1，vscode的传统艺能 */
        nlohmann::json start {
            { "line", lineno - 1 },
            { "character", linelength }
        };
        nlohmann::json end {
            { "line", lineno },
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
            { "name", name },
            { "kind", kind },
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

    // -----debug-----
    fmt::print(debug_file, ">>> Sending server response:\n{}\n\n",
               resp_content.dump(4));
    debug_file.flush();
    // ---------------
}
