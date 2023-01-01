#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <future>
#include <string>
#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>
#include <fstream>

#include "../server/nlohmann/json.hpp"

// asynchronous io version
int main(int argc, char *argv[])
{
    std::string path = "/home/ajin/Desktop/ajpj/cpppls/test/main.pl";

    // 0.统计文件中每行的字符数
    std::ifstream mystream(path);
    std::string li;
    std::vector<int> lines_len;
    while (std::getline(mystream, li)) {
        lines_len.push_back(li.length());
    }

    boost::asio::io_service ios;
    std::future<std::string> data;
    std::string cmd = "perl -c " + path;
    boost::process::child c(cmd,
                            boost::process::std_out > boost::process::null,
                            boost::process::std_err > data,
                            ios);

    ios.run();
    std::string err = data.get();
    std::string delimiter = "\n";
    size_t pos = 0;
    int i = 1;
    std::vector<nlohmann::json> diagnostics_vec;
    while ((pos = err.find(delimiter)) != std::string::npos) {
        // 1.获取当前行
        std::string line = err.substr(0, pos);
        err.erase(0, pos + delimiter.length());

        // 2.处理 "syntax OK" 和 “Subroutine xxx redefined” 的场景(即不做处理)
        std::regex syntax_ok("syntax OK");
        std::regex subroutine_redefined("Subroutine .+ redefined");
        if (std::regex_search(line, syntax_ok) ||
            std::regex_search(line, subroutine_redefined)) {
            continue;
        }

        // 3.处理存在语法错误的位置
        std::smatch sm;
        std::regex syntax_error("^(.+) at (.+?) line (\\d+)(, .+)?");
        if (std::regex_search(line, sm, syntax_error)) {
            std::string error = sm[0];
            std::string file = sm[1];
            int line = std::stoi(sm[2]);
            std::string area = sm[3];
            if (file != path) continue;
            if (area.length()) {
                error += area;
            }
            nlohmann::json start{
                { "line", line },
                { "character", 0 }
            };
            nlohmann::json end{
                { "line", line },
                { "character", lines_len[line - 1] }
            };
            nlohmann::json range{
                { "start", start },
                { "end", end },
            };
            nlohmann::json diagnostic{
                { "range", range },
                { "message", error },
                { "severity", 1 },
                { "source", "perl" }
            };
            diagnostics_vec.push_back(diagnostic);
        }
    }

    nlohmann::json diagnostics(diagnostics_vec);
    nlohmann::json params{
        { "uri", path },
        { "diagnostics", diagnostics }
    };
    nlohmann::json resp_content{
        { "jsonrpc", "2.0" },
        { "method", "textDocument/publishDiagnostics" },
        { "params", params },
        { "notification", 1 }
    };
}
