#ifndef CPPPLS_URI_H
#define CPPPLS_URI_H

#include <string>

class URI {
public:
    URI(const std::string&);
    ~URI() = default;

    std::string get_scheme();
    std::string get_path();

private:
    const std::string m_raw_str;
    std::string::const_iterator m_cur;
    std::string m_scheme;
    std::string m_authority;
    std::string m_path;

    // std::string m_query;
    // std::string m_fragment;

    void parse_scheme();
    void parse_authority();
    void parse_path();

    // 20221211: 不关注query和fragment
    // void parse_query();
    // void parse_fragment();
};

#endif
