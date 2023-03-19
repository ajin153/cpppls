#ifndef CPPPLS_HANDLER_H
#define CPPPLS_HANDLER_H

#ifndef NDEBUG
// -----debug-----
#include <fstream>
extern std::ofstream debug_file;
// ---------------
#endif

#include <string>
#include <iostream>

// third-party
#define FMT_HEADER_ONLY
#include "../fmt/format.h"
#include "../fmt/ostream.h"
#include "../nlohmann/json.hpp"

// @brief: 处理client request和server request的通用基类
class Handler {
public:
    Handler() = default;

    virtual ~Handler() = default;

    virtual void handle(const nlohmann::json&) {}

    std::string make_message(const nlohmann::json& msg_content)
    {
        std::string msg_header;
        // dump()必须使用-1(默认值)，不然lsp-client无法识别response
#ifdef __linux__
        static const std::string endflag = "\r\n";
#elif _WIN32
        static const std::string endflag = "\n";
#endif
        msg_header.append("Content-Length: " +
            std::to_string(msg_content.dump().size()) + endflag);
        msg_header.append(endflag);
        return msg_header + msg_content.dump();
    }

    void send_message(const std::string& message)
    {
        fmt::print("{}", message);
        std::cout << std::flush;
    }
};

#endif