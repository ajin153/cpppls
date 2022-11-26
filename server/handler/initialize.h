#ifndef CPPPLS_INITIALIZE_HANDLER_H
#define CPPPLS_INITIALIZE_HANDLER_H

#include "request.h"
#include <optional>

// method : Initialize
class InitializeHandler : public RequestHandler {
public:
    InitializeHandler() = default;
    ~InitializeHandler() = default;
    std::optional<std::string> handle(const nlohmann::json&) override;
};

#endif