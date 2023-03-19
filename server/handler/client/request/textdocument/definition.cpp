#include "definition.h"
#include "../../../../parser/uri.h"
#include "../../../../tools/tools.h"
#include <regex>
#include <unordered_map>

/*
{
    "id": 7,
    "jsonrpc": "2.0",
    "method": "textDocument/definition",
    "params": {
        "position": {
            "character": 26,
            "line": 4705
        },
        "textDocument": {
            "uri": "file:///d%3A/Users/User/Desktop/sf_project/z_tmp/cpppls/test/VGpuManager.pm"
        }
    }
}
*/

void
DefinitionHandler::handle(const nlohmann::json &req_content)
{
    // 0.g_perl_indexs如果还没有准备好，那就不支持Go to definition
    if (!Tools::is_file_index_initialized()) return;

    // 1.获取请求跳转的内容
    std::string raw_uri = req_content["params"]["textDocument"]["uri"];
    URI uri = URI(raw_uri);
    std::string perl_file_path = uri.get_path();
    if (!Tools::is_file_content_exist(perl_file_path)) return;
    if (!Tools::is_file_symbol_exist(perl_file_path)) return;

    const FileSymbol &file_symbol = Tools::get_file_symbol(perl_file_path);
    const std::vector<FileLine> &file_lines = Tools::get_file_content(perl_file_path);

    int row = req_content["params"]["position"]["line"];
    int col = req_content["params"]["position"]["character"];
    FileLine file_line = file_lines[row];
    std::string cur_line = file_line.m_content;
    std::regex definition_regex("[a-zA-Z0-9:_>-]+"); // use ">-" not "->"
    if (col >= file_line.m_length) return;
    int left = col, right = col + 1;
    while (left > 0) {
        if (std::regex_match(cur_line.substr(left, 1), definition_regex)) {
            --left;
        } else {
            break;
        }
    }
    while (right < file_line.m_length) {
        if (std::regex_match(cur_line.substr(right, 1), definition_regex)) {
            ++right;
        } else {
            break;
        }
    }
    if (left >= right) return;
    std::string definition_str = cur_line.substr(left + 1, right - left - 1);

    // 2.如果存在"::"，说明需要跳转的是外部函数，否则在同文件内跳转
    size_t pos = 0;
    std::string package_name;
    std::string function_name;
    size_t definition_len = definition_str.size();

    if ((pos = definition_str.find_last_of("::")) != std::string::npos) {
        size_t arrow_pos = 0;
        if ((arrow_pos = definition_str.find("->")) != std::string::npos) {
            // 跳转TEST::AJIN->test()类型的test()
            package_name = definition_str.substr(0, arrow_pos);
            function_name = definition_str.substr(arrow_pos + 2);
        } else {
            // 跳转TEST::AJIN::test()类型的test()
            package_name = definition_str.substr(0, pos - 1);
            function_name = definition_str.substr(pos + 1);
        }
    } else {
        // 跳转当前文件的test()
        package_name = file_symbol.m_package_name;
        function_name = definition_str;
    }

    // 3.获取 g_file_indexs，判断需要跳转的字段是否存在
    if (!Tools::is_definition_index_exist(package_name, function_name)) return;
    PackageIndex package_index = Tools::get_package_index(package_name);
    const std::unordered_map<std::string, FunctionIndex> &function_indexs =
        package_index.m_function_indexs;
    const FunctionIndex &function_index = function_indexs.at(function_name);

    // 4.封装response
    std::string res_uri = URI::encode_uri_path(package_index.m_uri);
    nlohmann::json start {
        { "line", function_index.m_lineno - 1 },
        { "character", function_index.m_length }
    };
    nlohmann::json end{
        { "line", function_index.m_lineno },
        { "character", 0 }
    };
    nlohmann::json range{
        { "start", start },
        { "end", end }
    };
    nlohmann::json definition_location{
        { "uri", res_uri },
        { "range", range }
    };
    nlohmann::json resp_content{
        { "jsonrpc", "2.0" },
        { "id", req_content["id"] },
        { "result", definition_location }
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