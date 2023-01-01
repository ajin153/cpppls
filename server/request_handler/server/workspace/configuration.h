#ifndef CPPPLS_WORKSPACE_CONFIGURATION_H
#define CPPPLS_WORKSPACE_CONFIGURATION_H

#include "../../handler.h"

extern std::string g_perl_exe;
extern std::vector<std::string> g_perl_incs;

// @brief: workspace/configuration
class Configuration: public RequestHandler {
public:
    Configuration() = default;
    ~Configuration() = default;
    void handle(const nlohmann::json&) override;
};

#endif
