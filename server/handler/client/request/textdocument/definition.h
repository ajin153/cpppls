#ifndef CPPPLS_TEXTDOCUMENT_DEFINITION_H
#define CPPPLS_TEXTDOCUMENT_DEFINITION_H

#include "../../../handler.h"
#include <unordered_map>

/*
 * [client request]
 * method: textdocument/definition
 */
class DefinitionHandler: public Handler {
public:
    DefinitionHandler() = default;
    ~DefinitionHandler() = default;
    void handle(const nlohmann::json&) override;
};

#endif