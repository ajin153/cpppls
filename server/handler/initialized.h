#ifndef CPPPLS_INITIALIZED_HANDLER_H
#define CPPPLS_INITIALIZED_HANDLER_H

#include "request.h"
#include <optional>

// method : Initialized
class InitializedHandler: public RequestHandler {
public:
    InitializedHandler() {}
    ~InitializedHandler() = default;
    std::optional<std::string> handle(const nlohmann::json&) override;
};

#endif