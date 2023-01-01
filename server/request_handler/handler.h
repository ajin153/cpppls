#ifndef CPPPLS_HANDLER_H
#define CPPPLS_HANDLER_H

// -----debug-----
#include <fstream>
extern std::ofstream debug_file;
// ---------------

#include <string>
#include <optional>
#include <iostream>

// third-party
#define FMT_HEADER_ONLY
#include "../fmt/format.h"
#include "../fmt/ostream.h"
#include "../nlohmann/json.hpp"

// @brief: 处理client request和server request的通用基类
class RequestHandler {
public:
    RequestHandler() = default;

    virtual ~RequestHandler() = default;

    virtual void handle(const nlohmann::json&) {}

    std::string make_response(const nlohmann::json& resp_content)
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

    void send_response(const std::string& response)
    {
        fmt::print("{}", response);
        std::cout << std::flush;
    }
};

#endif
