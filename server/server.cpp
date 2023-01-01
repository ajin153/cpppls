#include "request_handler/handler.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

// third-party
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/ostream.h"

#include "server.h"
#include "message_buffer.h"
#include "factory/request_handler.h"
#include "factory/lifecycle.h"

// -----debug-----
std::ofstream debug_file;
// ---------------

// @brief: server向client发送请求时使用的id (递增)
int g_server_request_id = 0;

// @brief: perl二进制文件的位置，用于perl -c
std::string g_perl_exe;

// @brief: perl的@INC，用于perl -c
std::vector<std::string> g_perl_incs;

// @brief: 保存server向client发送的请求handler，用于处理client的response
std::unordered_map<int, RequestHandler*> g_server_requests;

/*
 * @brief: cpppls启动入口
 */
void
Server::run()
{

    // -----debug-----
#ifdef _WIN32
    debug_file.open("D:/Users/User/Desktop/sf_project/z_tmp/cpppls/server/out/cpppls.log",
                    std::ios_base::app);
#elif __linux__
    debug_file.open("/home/ajin/Desktop/ajpj/cpppls/server/out/cpppls.log",
                    std::ios_base::app);
#endif
    // ---------------

    char c;
    MessageBuffer msg_buffer;
    while (std::cin.get(c)) {
        msg_buffer.handle_char(c);
        if (msg_buffer.parse_completed()) {
            auto msg_content = msg_buffer.get_m_msg_content();
            if (msg_content.contains("method")) {
                // client request
                std::string method = msg_content["method"];
                std::string method_prefix;

                // -----debug-----
                fmt::print(debug_file, ">>> Received client request:\n{}\n\n",
                           msg_content.dump(4));
                debug_file.flush();
                // ---------------

                auto delim_pos = method.find("/");
                if (delim_pos != std::string::npos) {
                    method_prefix = method.substr(0, delim_pos);
                    method = method.substr(delim_pos);
                }
                RequestHandlerFactory *factory;
                if (method == "initialize" ||
                    method == "initialized") {
                    factory = new LifeCycleFactory(method);
                } else {
                    factory = new RequestHandlerFactory();
                }
                RequestHandler *handler = factory->create_handler();
                handle_client_request(handler, msg_content);
                delete handler;
                delete factory;
            } else {
                // client response (for server request)

                // -----debug-----
                fmt::print(debug_file, ">>> Received client response:\n{}\n\n",
                           msg_content.dump(4));
                debug_file.flush();
                // ---------------
                if (msg_content.contains("id")) {
                    int id = msg_content["id"];
                    if (g_server_requests.count(msg_content["id"])) {
                        RequestHandler *handler = g_server_requests[id];
                        handle_client_response(handler, msg_content);
                        delete handler;
                        g_server_requests.erase(id);
                    }
                }
            }
            msg_buffer.clear();
        }
    }

    // -----debug-----
    if (debug_file)
        debug_file.close();
    // ---------------

}

/*
 * @brief: 处理client的request
 */
void
Server::handle_client_request(RequestHandler* handler,
                      const nlohmann::json& req_content)
{
    handler->handle(req_content);
}

/*
 * @brief: 处理client的response (对应server request的返回值)
 */
void
Server::handle_client_response(RequestHandler* handler,
                      const nlohmann::json& resp_content)
{
    handler->handle(resp_content);
}
