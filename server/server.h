#ifndef CPPPLS_SERVER_H
#define CPPPLS_SERVER_H

#include <optional>
#include <string>

#include "request_handler/handler.h"

class Server {
public:
    void run();

private:
    void handle_client_request(RequestHandler*, const nlohmann::json&);

    void handle_client_response(RequestHandler*, const nlohmann::json&);
};

#endif
