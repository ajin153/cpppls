#include "configuration.h"
#include "../../../../parser/uri.h"
#include "../../../../tools/tools.h"
#include <boost/asio/io_service.hpp>
#include <boost/process/io.hpp>
#include <filesystem>
#include <functional>
#include <future>

// third-party
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <regex>

/* example resp_content
{
    "id": 1,
    "jsonrpc": "2.0",
    "result": [
        {
            "perl_exe": "perl",
            "perl_inc": [
                "/home/ajin/Desktop/ajpj/cpppls/test/",
                "/usr/share/perl5/"
            ],
            "perl_scanner": {
                "enabled": true,
                "path": "d:/Users/User/Desktop/ajpj/z_tmp/cpppls/server/tools/perl_scanner/"
            }
        }
    ]
}
*/
void
ConfigurationHandler::handle(const nlohmann::json& resp_content)
{
    // 设置g_perl_exe和g_perl_incs
    std::vector<nlohmann::json> results;
    for (auto &result : resp_content["result"]) {
        results.push_back(result);
    }
    if (results.empty()) return;
    nlohmann::json result = results[0];
    std::string perl_exe;
    if (result.count("perl_exe") != 0) {
        perl_exe = result["perl_exe"];
        Tools::set_perl_exe(perl_exe);
    }
    std::vector<std::string> perl_incs;
    if (result.count("perl_incs") != 0) {
        for (auto& inc : result["perl_incs"]) {
            perl_incs.push_back(inc);
        }
        Tools::set_perl_incs(perl_incs);
    }

    // 设置g_perl_scanner_enabled和g_perl_scanner_path
    if (result.count("symbol_scanner") == 0) return;

    if (result["symbol_scanner"].count("enabled") == 0) return;

    if (result["symbol_scanner"]["enabled"] == true) {
        Tools::set_symbol_scanner_enabled(true);
    } else {
        Tools::set_symbol_scanner_enabled(false);
        return;
    }

    if (result["symbol_scanner"]["enable"] == true &&
        result["symbol_scanner"].count("path") == 0) return;

    std::string symbol_scanner_path = result["symbol_scanner"]["path"];
    if (symbol_scanner_path.back() == '/') {
        symbol_scanner_path.pop_back();
    }
#ifdef _WIN32
    symbol_scanner_path += "/symbol_scanner.exe";
#elif __linux__
    symbol_scanner_path += "/symbol_scanner";
#endif
    Tools::set_symbol_scanner_path(symbol_scanner_path);

    // g_perl_indexs 的生成强依赖与 symbol_scanner，必须正确设置 symbol_scanner
    if (!Tools::is_symbol_scanner_enabled()) return;

    // 如果 g_perl_incs 没有数据，不做处理
    if (Tools::is_perl_incs_empty()) return;

    // 使用后台线程对所有g_perl_incs下的.pl和.pm文件生成索引(耗时操作)
    std::thread init_index_t(&Tools::initialize_file_index);
    init_index_t.detach();
}