#ifndef CPPPLS_INITIALIZED_HANDLER_H
#define CPPPLS_INITIALIZED_HANDLER_H

#include <unordered_map>

#include "../handler.h"

extern int g_server_request_id;
extern std::unordered_map<int, RequestHandler*> g_server_requests;

// method : Initialized
class InitializedHandler: public RequestHandler {
public:
    InitializedHandler() {}
    ~InitializedHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif
