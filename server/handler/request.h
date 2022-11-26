#ifndef CPPPLS_REQUEST_HANDLER_H
#define CPPPLS_REQUEST_HANDLER_H

#include <string>
#include <optional>
#include <iostream>

#include "../nlohmann/json.hpp"

class RequestHandler {
public:
    RequestHandler() {}

    virtual ~RequestHandler() = default;

    virtual std::optional<std::string> handle(const nlohmann::json &req_content)
    {
        return std::nullopt;
    }

    std::string make_response(const nlohmann::json &resp_content)
    {
        std::string resp_header;
        // dump()必须使用-1(默认值)，不然lsp-client无法识别response
#ifdef __linux__
        static const std::string endflag = "\r\n";
#elif _WIN32
        static const std::string endflag = "\n";
#endif
        resp_header.append("Content-Length: " +
            std::to_string(resp_content.dump().size()) + endflag);
        resp_header.append(endflag);
        return resp_header + resp_content.dump();
    }

// -----debug-----
public:
    nlohmann::json m_resp_content;
    nlohmann::json get_resp_content()
    {
        return m_resp_content;
    }
// ---------------

};

#endif
