#ifndef CPPPLS_WORKSPACE_CONFIGURATION_H
#define CPPPLS_WORKSPACE_CONFIGURATION_H

#include "../../../handler.h"

extern std::string g_perl_exe;
extern std::vector<std::string> g_perl_incs;

// [server request]
// method: workspace/configuration
class ConfigurationHandler: public Handler {
public:
    ConfigurationHandler() = default;
    ~ConfigurationHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif
