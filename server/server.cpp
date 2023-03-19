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
#include "factory/handler.h"
#include "factory/lifecycle.h"
#include "factory/textdocument.h"

#ifndef NDEBUG
// -----debug-----
std::ofstream debug_file;
// ---------------
#endif

// @brief: server向client发送请求时使用的id (递增)
int g_server_request_id = 0;

// @brief: 保存server向client发送的请求handler，用于处理client的response
std::unordered_map<int, Handler*> g_server_requests;

/*
 * @brief: cpppls启动入口
 */
void
Server::run()
{
#ifndef NDEBUG
// -----debug-----
#ifdef _WIN32
    debug_file.open("C:/Users/29280/Desktop/ajpj/cpppls/server/out/cpppls.log",
                    std::ios_base::app);
#elif __linux__
    debug_file.open("/home/ajin/Desktop/ajpj/cpppls/server/out/cpppls.log",
                    std::ios_base::app);
#endif
// ---------------
#endif

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

#ifndef NDEBUG
                // -----debug-----
                fmt::print(debug_file, ">>> Received client request:\n{}\n\n",
                           msg_content.dump(4));
                debug_file.flush();
                // ---------------
#endif

                auto delim_pos = method.find("/");
                if (delim_pos != std::string::npos) {
                    method_prefix = method.substr(0, delim_pos);
                    method = method.substr(delim_pos+1);
                }
                HandlerFactory *factory;
                if (method == "initialize" ||
                    method == "initialized") {
                    factory = new LifeCycleFactory(method);
                } else if (method_prefix == "textDocument") {
                    factory = new TextDocumentFactory(method);
                } else {
                    factory = new HandlerFactory();
                }
                Handler *handler = factory->create_handler();
                handle_message(handler, msg_content);
                delete handler;
                delete factory;
            } else {
                // client response (for server request)

#ifndef NDEBUG
                // -----debug-----
                fmt::print(debug_file, ">>> Received client response:\n{}\n\n",
                           msg_content.dump(4));
                debug_file.flush();
                // ---------------
#endif

                if (msg_content.contains("id")) {
                    int id = msg_content["id"];
                    if (g_server_requests.count(msg_content["id"])) {
                        Handler *handler = g_server_requests[id];
                        handle_message(handler, msg_content);
                        delete handler;
                        g_server_requests.erase(id);
                    }
                }
            }
            msg_buffer.clear();
        }
    }

#ifndef NDEBUG
    // -----debug-----
    if (debug_file)
        debug_file.close();
    // ---------------
#endif

}

/*
 * @brief: 处理client的request和response
 */
void
Server::handle_message(Handler* handler, const nlohmann::json& msg_content)
{
    handler->handle(msg_content);
}
