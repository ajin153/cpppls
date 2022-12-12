#include "uri.h"
#include <cctype>
#include <cstddef>
#include <stdexcept>

URI::URI(const std::string &raw_str): m_raw_str(raw_str)
{
    m_cur = m_raw_str.cbegin();
    parse_scheme();
    parse_authority();
    parse_path();

    // parse_query();
    // parse_fragment();
}

/*
 * Scheme names consist of a sequence of characters beginning with a
 * letter and followed by any combination of letters, digits, plus
 * ("+"), period ("."), or hyphen ("-").
 */
void URI::parse_scheme()
{
    std::string::const_iterator str_cur = m_cur;
    std::string::const_iterator str_end = m_raw_str.cend();
    if (str_cur == str_end) {
        // 优化：这里抛出的异常，外部识别并捕捉到后应该放弃本次请求的解析，
        // 而不是直接终止cpppls的运行
        throw std::runtime_error("[error]: parse_scheme() failed,"
                                 " raw_str is null!");
    }
    // 1.judge the beginning
    if (!std::isalpha(*str_cur)) {
        throw std::runtime_error("[error]: parse_scheme() failed,"
                                 " raw_str contains unstandard scheme!");
    }
    str_cur += 1;
    // 2.judge the followed
    while(str_cur != str_end && *str_cur != ':') {
        if (!std::isalpha(*str_cur) && !std::isalnum(*str_cur) &&
            (*str_cur != '+' || *str_cur != '-' || *str_cur != '.')) {
            throw std::runtime_error("[error]: parse_scheme() failed,"
                                     " raw_str contains unstandard scheme!");
        }
        str_cur += 1;
    }
    // 3.judge if only scheme data
    if (str_cur == str_end) {
        throw std::runtime_error("[error]: parse_scheme() failed,"
                                 " it seems raw_str is unstandard"
                                 " (only has scheme)!");
    }
    m_scheme = std::string(m_cur, str_cur);
    // *str_cur = ':'
    m_cur = str_cur + 1;
}

// 20221211: 不关注username,password,host和ip的具体内容
void URI::parse_authority()
{
    std::string::const_iterator str_end = m_raw_str.cend();
    std::string content = std::string(m_cur, str_end);
    if (!content.compare(0, 2, "//")) {
        std::string::const_iterator authority_cur = m_cur + 2;
        // 20221211: 只关注获取后续的path
        while (authority_cur != str_end && *authority_cur != '/') {
            authority_cur += 1;
        }
        if (authority_cur == str_end) {
            throw std::runtime_error("[error]: parse_authority() failed,"
                                     " it seems raw_str is unstandard"
                                     " (authority has no ending!)");
        } else if (*authority_cur == '/') {
            m_authority = std::string(m_cur + 2, authority_cur);
            m_cur = authority_cur;
        }
    } else if (!content.compare(0, 1, "/")) {
        m_authority = "";
    } else {
        throw std::runtime_error("[error]: parse_authority() failed,"
                                 " it seems raw_str is unstandard"
                                 " (authority has wrong beginning!)");
    }
#ifdef _WIN32
    /*
     * windows的uri实例:
     *     file:///d%3A/Users/User/Desktop/sf_project/z_tmp/cpppls/test/test.pl
     * /d:/xxxx 这种 path 无法识别，需要手动将最前面的 '/' 跳过，即解析：
     * d:/xxx 这种 path
     */
    m_cur += 1;
#endif
}

void URI::parse_path()
{
    std::string::const_iterator str_cur = m_cur;
    std::string::const_iterator str_end = m_raw_str.cend();
    while (str_cur != str_end && *str_cur != '?' && *str_cur != '#') {
        str_cur += 1;
    }
    m_path = std::string(m_cur, str_cur);
    decode_uri_path();
}

// 20221212: 解码path中类似 ":" 这类使用16进制表示的字符
void URI::decode_uri_path()
{
    std::string res = "";
    std::size_t len = m_path.length();
    for (std::size_t i = 0; i < len; ++i) {
        if (m_path[i] == '%') {
            if (i + 2 >= len) {
                throw std::runtime_error("[error]: decode_uri_path() failed,"
                                         " it seems path contains unstandard"
                                         " percent encodings!");
            }
            unsigned char high = from_hex(m_path[++i]);
            unsigned char low = from_hex(m_path[++i]);
            res += high * 16 + low;
        } else {
            res += m_path[i];
        }
    }
    m_path = res;
}

// 20221212: 解码path中类似 ":" 这类使用16进制表示的字符
unsigned char URI::from_hex(const unsigned char &x)
{
    unsigned char res;
    if (x >= 'A' && x <= 'Z') {
        res = x - 'A' + 10;
    } else if (x >= 'a' && x <= 'z') {
        res = x - 'a' + 10;
    } else if (x >= '0' && x <= '9') {
        res = x - '0';
    } else {
        throw std::runtime_error("[error]: from_hex() failed,"
                                 " it seems you passed the character which"
                                 " is not in [a-z], [A-Z] or [0-9]!");
    }
    return res;
}

// 获取解析好的uri:scheme
std::string URI::get_scheme()
{
    return m_scheme;
}

// 获取解析好的uri:path
std::string URI::get_path()
{
    return m_path;
}
