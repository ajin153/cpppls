#ifndef CPPPLS_INITIALIZE_HANDLER_H
#define CPPPLS_INITIALIZE_HANDLER_H

#include "../handler.h"

// method : Initialize
class InitializeHandler : public RequestHandler {
public:
    InitializeHandler() = default;
    ~InitializeHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif
