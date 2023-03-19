#ifndef CPPPLS_WORKSPACE_CONFIGURATION_H
#define CPPPLS_WORKSPACE_CONFIGURATION_H

#include "../../../handler.h"
#include <unordered_map>

/*
 * [server request]
 * method: workspace/configuration
 */
class ConfigurationHandler: public Handler {
public:
    ConfigurationHandler() = default;
    ~ConfigurationHandler() = default;
    void handle(const nlohmann::json&) override;
    void initialize_file_index(std::vector<std::string>);
};

#endif