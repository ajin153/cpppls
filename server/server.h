#ifndef CPPPLS_SERVER_H
#define CPPPLS_SERVER_H

#include <optional>
#include <string>

#include "handler/handler.h"

class Server {
public:
    void run();

private:
    void handle_message(Handler*, const nlohmann::json&);
};

#endif
