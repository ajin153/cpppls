#ifndef CPPPLS_TOOLS_H
#define CPPPLS_TOOLS_H

#include <string>
#include <unordered_map>
#include <vector>

typedef std::string FunctionName;

struct FunctionIndex {
    int m_lineno;    // index start lineno
    int m_length; // index length
};

struct PackageIndex {
    std::string m_uri;
    std::unordered_map<FunctionName, FunctionIndex> m_function_indexs;
};

struct FileLine {
    std::string m_content; // the content of this line
    int m_length;  // the length of this line
};

struct SymbolInfo {
    std::string m_name;
    int m_kind;
    int m_lineno;
    int m_length;
};

struct FileSymbol {
    std::string m_package_name; // 这是为了服务 Go to definition 的同文件跳转增加的冗余数据
    std::vector<SymbolInfo> m_symbols;
};

// static class
// provide common api, tools
class Tools {
public:

    static bool is_symbol_scanner_enabled();

    static bool is_file_content_exist(const std::string&);

    static bool is_file_symbol_exist(const std::string&);

    static bool is_file_index_initialized();

    static bool is_perl_incs_empty();

    static bool is_definition_index_exist(const std::string&, const std::string&);

    static const std::vector<FileLine>& get_file_content(const std::string&);

    static const FileSymbol& get_file_symbol(const std::string&);

    static const std::string& get_perl_exe();

    static const std::vector<std::string>& get_perl_incs();

    static const PackageIndex& get_package_index(const std::string&);

    // index 和 symbol 不应该是同个概念，因为 index 应该要表现出 package 和 function
    // 之间的层级关系（因为.pm允许存在多个package声明），而 symbol 则只需要记录
    // 在文件中一个 symbol 出现的位置即可
    // 不过我们这里先做一个简单处理，使用symbol来实现Go to definition(后续需要改成用index)
    // 这样就需要给file_index加一个限制，我们仅取每个文件中出现的第一个package作为代表该文件
    // 的PackageName(这样会处理不了有多个package定义的情况，具体可看: https://stackoverflow.com/questions/1748896/in-perl-how-do-i-put-multiple-packages-in-a-single-pm-file)
    static void update_file_symbol_and_index(const std::string&);

    static void update_file_content(const std::string&);

    static void set_perl_exe(const std::string&);

    static void set_perl_incs(const std::vector<std::string>&);

    static void set_symbol_scanner_enabled(bool);

    static void set_symbol_scanner_path(const std::string&);

    static void initialize_file_index();
};

#endif