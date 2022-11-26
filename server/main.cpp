#include <iostream>
#include <fstream>

// third-party
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/ostream.h"

// ajin
#include "request_buffer.h"
#include "factory/lifecycle.h"
#include "factory/textdocument.h"
#include "factory/workspace.h"
#include "factory/completionitem.h"

static std::ofstream debug_file;

int main(int argc, char *argv[])
{

    // -----debug-----
    debug_file.open("/home/ajin/Desktop/ajpj/cpppls/server/out/cpppls.log", std::ios_base::app);
    // ---------------

    char c;
    RequestBuffer req_buf;
    while (std::cin.get(c)) {
        req_buf.handle_char(c);
        if (req_buf.parse_completed()) {
            auto req_content = req_buf.get_m_req_content();
            std::string method = req_content["method"];

            // -----debug-----
            fmt::print(debug_file, ">>> Received request:\n{}\n\n",
                       req_content.dump(4));
            debug_file.flush();
            // ---------------

            auto delim_pos = method.find("/");
            if (delim_pos != std::string::npos) {
                method = method.substr(0, delim_pos);
            }
            RequestHandlerFactory *factory;
            if (method == "initialize" ||
                method == "initialized") {
                factory = new LifeCycleFactory(method);
            } else {
                factory = new RequestHandlerFactory();
            }
            RequestHandler *handler = factory->create_handler();
            auto response = handler->handle(req_content);
            if (response.has_value()) {
                fmt::print("{}", response.value());
                std::cout << std::flush;

                // -----debug-----
                fmt::print(debug_file, "<<< Sending response:\n{}\n\n",
                           handler->get_resp_content().dump(4));
                // ---------------

            }
            req_buf.clear();
            delete factory;
            delete handler;
        }
    }

    // -----debug-----
    if (debug_file)
        debug_file.close();
    // ---------------

    return 0;
}
