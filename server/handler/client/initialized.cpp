#include "initialized.h"
#include "../server/request/workspace/configuration.h"

/*
 * client: initialized
 * server: workspace/configuration
 */
void InitializedHandler::handle(const nlohmann::json &req_content)
{
    // "workspace/configuration" server request
    // 1.发送请求
    auto items = nlohmann::json::array();
    nlohmann::json item{
        { "section", "cpppls" },
    };
    items.push_back(item);
    nlohmann::json params{
        { "items", items },
    };
    nlohmann::json get_configuration_req_content{
        { "jsonrpc", "2.0" },
        { "method", "workspace/configuration" },
        { "params", params },
    };
    get_configuration_req_content["id"] = ++g_server_request_id;

    std::string get_configuration_request =
        make_message(get_configuration_req_content);
    send_message(get_configuration_request);

    // -----debug-----
    fmt::print(debug_file, ">>> Sending server request:\n{}\n\n",
               get_configuration_req_content.dump(4));
    // ---------------

    // 2.保存请求handler
    Handler *configuration_handler = new ConfigurationHandler();
    g_server_requests[g_server_request_id] = configuration_handler;
}
