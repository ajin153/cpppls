#ifndef CPPPLS_INITIALIZE_HANDLER_H
#define CPPPLS_INITIALIZE_HANDLER_H

#include "../../handler.h"

/*
 * [client request]
 * method: Initialize
 */
class InitializeHandler : public Handler {
public:
    InitializeHandler() = default;
    ~InitializeHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif
