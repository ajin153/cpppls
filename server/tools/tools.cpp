#include "tools.h"
#include <fstream>
#include <future>
#include <regex>
#include <filesystem>

// third-party
#include <boost/process.hpp>
#include <boost/asio.hpp>

typedef std::string PackageName;
typedef std::string FilePath;

bool g_file_index_initialized = false;

std::unordered_map<PackageName, PackageIndex> g_file_indexs;

std::unordered_map<FilePath, FileSymbol> g_file_symbols;

std::unordered_map<FilePath, std::vector<FileLine>> g_file_contents;

bool g_symbol_scanner_enabled = false;

std::string g_symbol_scanner_exe = "";

std::string g_perl_exe = "";

std::vector<std::string> g_perl_incs;

/*
 * @brief: 检查 symbol_scanner 是否为可用状态
 * @return: true or false
 */
bool
Tools::is_symbol_scanner_enabled()
{
    return g_symbol_scanner_enabled && (g_symbol_scanner_exe != "");
}

/*
 * @brief: 检查 g_file_contents 中是否有对应 path 的数据
 * @param: path [文件路径]
 * @return: true or false
 */
bool
Tools::is_file_content_exist(const std::string& path)
{
    if (g_file_contents.count(path) != 0) {
        return true;
    }
    return false;
}

/*
 * @brief: 检查 g_file_symbols 中是否有对应 path 的数据
 * @param: path [文件路径]
 * @return: true or false
 */
bool
Tools::is_file_symbol_exist(const std::string& path)
{
    if (g_file_symbols.count(path) != 0) {
        return true;
    }
    return false;
}

/*
 * @brief: 检查针对 g_perl_incs 下的所有文件 index 是否初始化完毕
 * @return: true or false
 */
bool
Tools::is_file_index_initialized()
{
    return g_file_index_initialized && !g_file_indexs.empty();
}

/*
 * @brief: 检查 g_perl_incs 是否为空
 * @return true or false
 */
bool
Tools::is_perl_incs_empty()
{
    return g_perl_incs.empty();
}

/*
 * @brief: 检查 g_file_indexs 中是否有对应 package_name 和 function_name
 *         的 index 信息
 * @param: package_name
 */
bool
Tools::is_definition_index_exist(const std::string& package_name,
                                 const std::string& function_name)
{
    if (g_file_indexs.count(package_name) == 0) {
        return false;
    } else {
        const PackageIndex &package_index = g_file_indexs[package_name];
        const std::unordered_map<std::string, FunctionIndex> func_indexs =
            package_index.m_function_indexs;
        if (func_indexs.count(function_name) == 0) return false;
    }
    return true;
}

/*
 * @brief: 获取 g_file_contents 中对应 path 的数据
 * @param: path [文件路径]
 * @return: file lines
 */
const std::vector<FileLine>&
Tools::get_file_content(const std::string& path)
{
    return g_file_contents[path];
}

/*
 * @brief: 获取 g_file_contents 中对应 path 的数据
 * @param: path [文件路径]
 * @return: symbols
 */
const FileSymbol&
Tools::get_file_symbol(const std::string& path)
{
    return g_file_symbols[path];
}

/*
 * @brief: 获取 perl.exe 的位置
 * @return: perl.exe path
 */
const std::string&
Tools::get_perl_exe()
{
    return g_perl_exe;
}

/*
 * @brief: 获取配置的 perl incs 数据
 * @return: perl incs
 */
const std::vector<std::string>&
Tools::get_perl_incs()
{
    return g_perl_incs;
}

/*
 * @brief: 获取 g_file_indexs 中对应 package_name 的 index 信息
 * @param: package_name
 */
const PackageIndex&
Tools::get_package_index(const std::string& package_name)
{
    return g_file_indexs[package_name];
}

/*
 * @brief: 更新 g_file_symbols 和 g_file_indexs 的统一接口，支持更新提供的
 *         文件路径的 symbol 和 index 信息
 * @param: path [文件路径]
 */
void
Tools::update_file_symbol_and_index(const std::string& path)
{
    if (!g_symbol_scanner_enabled || g_symbol_scanner_exe == "") {
        return;
    }
    std::string symbol_scan_cmd = g_symbol_scanner_exe;
    std::future<std::string> result;
    boost::asio::io_service ios;
    boost::process::child symbol_scanner_child(symbol_scan_cmd, path,
        boost::process::std_in.close(),
        boost::process::std_out > result,
        boost::process::std_err > boost::process::null,
        ios);
    ios.run();
    std::string perl_scanner_data = result.get();
    size_t pos = 0;
#ifdef _WIN32
    std::string delimiter = "\r\n";
#elif __linux__
    std::string delimiter = "\n";
#endif
    std::vector<SymbolInfo> symbols;
    std::string package_name = "";
    bool first_package_name = true;
    std::unordered_map<std::string, FunctionIndex> function_indexs;
    FileSymbol file_symbol;
    while ((pos = perl_scanner_data.find(delimiter)) != std::string::npos) {
        std::string line = perl_scanner_data.substr(0, pos);
        perl_scanner_data.erase(0, pos + delimiter.length());
        /*
         * line, example: TEST::AJIN,4,1,17
         * 说明：
         * 每一行中包含对应lsp协议中SymbolInformation类型所需要的数据
         * 分别为: name,kind,lineno,linelength
         * 其中lineno和linelength对应Range类型的start和character(Location->Range)
         */
        std::regex line_delimiter(",");
        std::vector<std::string> strs(std::sregex_token_iterator(line.begin(),
            line.end(), line_delimiter, -1), std::sregex_token_iterator());
        SymbolInfo symbol;
        symbol.m_name = strs[0];
        symbol.m_kind = std::stoi(strs[1]);
        symbol.m_lineno = std::stoi(strs[2]);
        symbol.m_length = std::stoi(strs[3]);
        symbols.push_back(symbol);
        // update index
        if (symbol.m_kind == 4 && first_package_name) {
            package_name = symbol.m_name;
            file_symbol.m_package_name = package_name;
            first_package_name = false;
        } else if (symbol.m_kind == 12) {
            FunctionIndex func_index;
            func_index.m_lineno = symbol.m_lineno;
            func_index.m_length = symbol.m_length;
            function_indexs[symbol.m_name] = func_index;
        }
    }
    file_symbol.m_symbols = symbols;
    g_file_symbols[path] = file_symbol;
    PackageIndex package_index;
    package_index.m_uri = path;
    package_index.m_function_indexs = function_indexs;
    g_file_indexs[package_name] = package_index;
}

/*
 * @brief: 更新 g_file_contents 的统一接口，支持更新提供的文件路径的 content 信息
 * @param: path [文件路径]
 */
void
Tools::update_file_content(const std::string& path)
{
    std::ifstream fs(path);
    std::string fs_line;
    std::vector<FileLine> file_lines;
    while (std::getline(fs, fs_line)) {
        FileLine fl;
        fl.m_content = fs_line;
        fl.m_length = fs_line.length();
        file_lines.push_back(fl);
    }
    g_file_contents[path] = file_lines;
}

/*
 * @brief: 设置 g_perl_exe
 * @param: perl.exe path
 */
void
Tools::set_perl_exe(const std::string& path)
{
    g_perl_exe = path;
}

/*
 * @brief: 设置 g_perl_incs
 * @param: perl incs
 */
void
Tools::set_perl_incs(const std::vector<std::string>& incs)
{
    g_perl_incs.assign(incs.begin(), incs.end());
}

/*
 * @brief: 设置 g_symbol_scanner_enabled
 * @param: enabled (是否启用)
 */
void
Tools::set_symbol_scanner_enabled(bool enabled)
{
    g_symbol_scanner_enabled = enabled;
}

/*
 * @brief: 设置 g_symbol_scanner_exe
 * @param: symbol scanner path
 */
void
Tools::set_symbol_scanner_path(const std::string& path)
{
    g_symbol_scanner_exe = path;
}

/*
 * @brief: 对g_perl_incs目录下的所有.pl和.pm文件生成对应Package和Function索引
 */
void
Tools::initialize_file_index()
{
    for (const auto& path : g_perl_incs) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            std::string file_extension = entry.path().extension().generic_string();
            if (file_extension != ".pl" && file_extension != ".pm") {
                continue;
            }
            std::string perl_file_path = entry.path().generic_string();
            Tools::update_file_symbol_and_index(perl_file_path);
        }
    }
    g_file_index_initialized = true;
}