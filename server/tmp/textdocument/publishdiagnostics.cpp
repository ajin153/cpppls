#include "publishdiagnostics.h"
#include "../../parser/uri.h"

#include <fstream>
#include <vector>

// third-party
#include <boost/process.hpp>
#include <boost/asio.hpp>

std::optional<std::string> PublishDiagnosticsHandler::handle(const nlohmann::json &req_content) {

    // 1.解析文件uri
    URI *uri = new URI(req_content["params"]["textDocument"]["uri"]);
    std::string path = uri->get_path();

    // 2.统计文件中每行的字符数量
    std::ifstream cntfile(path);
    std::string cntline;
    std::vector<int> lines_len;
    while (std::getline(cntfile, cntline)) {
        lines_len.push_back(cntline.length());
    }

    // 3.调用 perl -c -I@inc path 校验文件语法
    boost::asio::io_service ios;
    std::future<std::string> data;

    boost::process::child c(m_perl_exe,
                            )

    nlohmann::json params{
        { "uri", xxx },
        { "diagnostics", diagnostics }
    };

    nlohmann::json resp_content{
        { "jsonrpc", "2.0" },
        { "method", "textDocument/publishDiagnostics" },
        { "params", params },
        { "notification", 1 }
    };

    // -----debug-----
    m_resp_content = resp_content;
    // ---------------

    return make_response(resp_content);
}
