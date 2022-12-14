#include <future>
#include <iostream>
#include <fstream>
#include <regex>

#include "publishdiagnostics.h"
#include "../../../../parser/uri.h"

// third-party
#include <boost/process.hpp>
#include <boost/asio.hpp>

/*
{
    "jsonrpc": "2.0",
    "method": "textDocument/didOpen",
    "params": {
        "textDocument": {
            "languageId": "perl",
            "text": "use strict;\nuse warnings;\n\nmy $test = 1;\nmy $test = 1\nmy $test2 = 1;\n\n$test = $test3;\n",
            "uri": "file:///home/ajin/Desktop/ajpj/cpppls/test/main.pl",
            "version": 1
        }
    }
}
*/
void
PublishDiagnosticsHandler::handle(const nlohmann::json& req_content)
{
    // 1. parse uri
    URI uri = URI(req_content["params"]["textDocument"]["uri"]);
    std::string perl_file_path = uri.get_path();

    // 2. count file each line length
    std::ifstream fs(perl_file_path);
    std::string fs_li;
    std::vector<int> line_lengths;
    while (std::getline(fs, fs_li)) {
        line_lengths.push_back(fs_li.length());
    }

    // 3. make "perl -I xxx -c yyy" cmd
    std::string perl_syntax_cmd = g_perl_exe;
    for (auto &perl_inc : g_perl_incs) {
        perl_syntax_cmd += (" -I " + perl_inc);
    }
    perl_syntax_cmd += (" -c " + perl_file_path);

    // 4. run perl syntax cmd
    std::future<std::string> result;
    boost::asio::io_service ios;
    boost::process::child perl_syntax_child(perl_syntax_cmd,
        boost::process::std_in.close(),
        boost::process::std_out > boost::process::null,
        boost::process::std_err > result,
        ios);
    ios.run();
    std::string perl_syntax_data = result.get();
    size_t pos = 0;
#ifdef _WIN32
    std::string delimiter = "\r\n";
#elif __linux__
    std::string delimiter = "\n";
#endif
    std::vector<nlohmann::json> diagnostics_vec;
    std::regex syntax_ok("syntax OK");
    std::regex subroutine_redefined("Subroutine .+ redefined");
    while((pos = perl_syntax_data.find(delimiter)) != std::string::npos) {
        std::string line = perl_syntax_data.substr(0, pos);
        perl_syntax_data.erase(0, pos + delimiter.length());

        // 5. skip "syntax OK" and "Subroutine xxx redefined"
        if (std::regex_search(line, syntax_ok) ||
            std::regex_search(line, subroutine_redefined)) {
            continue;
        }

        // 6. deal with syntax error
        std::smatch sm;
        std::regex syntax_error("^(.+) at (.+?) line (\\d+)(, .+)?");
        if (std::regex_search(line, sm, syntax_error)) {
            std::string error = sm[1];
            std::string file = sm[2];
            int line = std::stoi(sm[3]);
            std::string area = sm[4];
            if (perl_file_path != file) {
                continue;
            }
            if (area.length()) {
                error += area;
            }
            /*
               ????????? line - 1 ???????????????????????????
               ?????????????????? perl -c ??????????????? ???????????????
               ??????????????????????????????????????????
               ????????????vscode-client??????????????????????????????????????????0?????????
            */
            nlohmann::json start{
                { "line", line - 1 },
                { "character", 0 }
            };
            nlohmann::json end{
                { "line", line - 1 },
                { "character", line_lengths[line - 1] }
            };
            nlohmann::json range{
                { "start", start },
                { "end", end }
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
        { "uri", req_content["params"]["textDocument"]["uri"] },
        { "diagnostics", diagnostics }
    };
    nlohmann::json publish_diagnostics_content{
        { "jsonrpc", "2.0" },
        { "method", "textDocument/publishDiagnostics" },
        { "params", params },
        { "notification", 1 }
    };

    std::string publish_diagnostics_notification =
        make_message(publish_diagnostics_content);
    send_message(publish_diagnostics_notification);

    // -----debug-----
    fmt::print(debug_file, ">>> Sending server notification:\n{}\n\n",
               publish_diagnostics_content.dump(4));
    // ---------------

}
