#ifndef CPPPLS_REQUEST_BUFFER_H
#define CPPPLS_REQUEST_BUFFER_H

#include <string>
#include <map>
#include <tuple>

#include "nlohmann/json.hpp"


class RequestBuffer {
public:
    void handle_char(char c)
    {
        m_raw_req += c;
        auto new_header = try_parse_header();
        if (!std::get<0>(new_header).empty()) {
            m_req_headers[std::get<0>(new_header)] = std::get<1>(new_header);
            m_raw_req.clear();
        }

        // A sole "\r\n"(linux) or "\n"(windows) is the separate between the
        // header block and the body block, but we don't need it.
#ifdef __linux__
        static const std::string endflag = "\r\n";
#elif _WIN32
        static const std::string endflag = "\n";
#endif
       if (m_raw_req == endflag) {
            m_raw_req.clear();
            m_is_header_done = true;
        }

        if (m_is_header_done) {
            auto content_length = std::stoi(m_req_headers["Content-Length"]);
            if (m_raw_req.length() == content_length) {
                m_req_content = nlohmann::json::parse(m_raw_req);
            }
        }
    }

    std::tuple<std::string, std::string> try_parse_header() const
    {
#ifdef __linux__
        static const std::string endflag = "\r\n";
#elif _WIN32
        static const std::string endflag = "\n";
#endif
        auto eol_pos = m_raw_req.find(endflag);
        if (eol_pos != std::string::npos) {
            std::string header_str = m_raw_req.substr(0, eol_pos);
            auto delim_pos = header_str.find(":");
            if (delim_pos != std::string::npos) {
                std::string header_name = header_str.substr(0, delim_pos);
                std::string header_val = header_str.substr(delim_pos + 1);
                return std::make_tuple(header_name, header_val);
            }
        }
        return std::make_tuple(std::string{}, std::string{});
    }

    bool parse_completed()
    {
        if (m_is_header_done && !m_req_content.empty()) {
            return true;
        }
        return false;
    }

    nlohmann::json get_m_req_content() const
    {
        return m_req_content;
    }

    void clear()
    {
        m_raw_req.clear();
        m_req_headers.clear();
        m_is_header_done = false;
        m_req_content.clear();
    }

    // debug
    std::string get_m_raw_req()
    {
        return m_raw_req;
    }
private:
    std::string m_raw_req;
    std::map<std::string, std::string> m_req_headers;
    bool m_is_header_done = false;
    nlohmann::json m_req_content;
};

#endif
