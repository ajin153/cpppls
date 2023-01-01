#include "configuration.h"

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
            ]
        }
    ]
}
*/
void Configuration::handle(const nlohmann::json& resp_content)
{
    // 设置g_perl_exe和g_perl_incs
    std::vector<nlohmann::json> results;
    for (auto &result : resp_content["result"]) {
        results.push_back(result);
    }
    nlohmann::json result = results[0];
    g_perl_exe = result["perl_exe"];
    for (auto &perl_inc : result["perl_incs"]) {
        g_perl_incs.push_back(perl_inc);
    }

    // -----debug-----
    std::string perl_incs_str = "[";
    for (auto &perl_inc : g_perl_incs) {
        perl_incs_str += ("\"" + perl_inc + "\"" + ", ");
    }
    perl_incs_str = perl_incs_str.substr(0, perl_incs_str.length()-2) + "]";
    fmt::print(debug_file, "--- g_perl_exe = \"{}\", g_perl_incs = {}\n\n",
               g_perl_exe, perl_incs_str);
    // ---------------

}
