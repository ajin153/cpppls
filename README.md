# Perl Language Server Written In C++ (cpppls)
A simple implementation of the language server protocol for perl5. (until now)

## Features
- Syntax checking
- Symbols in file
  - package
  - function
- Go to definition
  - TEST::TMP::test()
  - TEST::TMP->test()
  - test()
- Both Windows and Linux support

## Installation
### Requirement
- boost 1.8.0+ (recommended, I used the boost 1.8.0)
- cmake 3.5+ (I used the cmake 3.25.1)
- flex 2.5.4+ (I used the flex 2.5.4)
- gcc 11.2.0+ (I used the gcc 11.2.0)
- msvc (Only needed for windows user, I used visual studio 2019 and msvc 14.2)

### Steps
#### 1.Compile the boost
##### Windows 10

1.Download the boost from this page: [boost download page](https://www.boost.org/)

2.And then, maybe you can run the following command to compile it simply：

```bash
./bootstrap.bat
./b2.exe --layout=versioned variant=release debug -j 8 address-model=64 link=shared threading=multi --prefix=c:\Boost install
```
*Note: If you run into problems, you can get more help info from the boost download page.*

3.Set the environment variable for the boost lib and boost path.

##### Manjaro

On Manjaro, you can run this command to get the boost dependency simply.

```bash
pacman -S boost-libs extra/boost
```

#### 2.Compile the cpppls

1.Download the source code from release version pages.

| system | sourcecode |
| ------ | ---------- |
| Window 10 | cpppls-win64.zip |
| Manjaro | cpppls-linux64.tar.gz |

2.Make some adjustments to the content of this file: **CMakeLists.txt**

```cmake
...
if (WIN32)
    set(BOOST_ROOT C:/Boost)                                   # Set your boost path
    set(Boost_LIBRARY_DIRS C:/Boost/lib)                       # Set your boost lib path
...
if (UNIX)
    set(BOOST_ROOT /home/ajin/Desktop/ajlib/Boost)             # Set your boost path
    set(Boost_LIBRARY_DIRS /home/ajin/Desktop/ajlib/Boost/lib) # Set your boost lib path
...
find_package(Boost 1.80 REQUIRED COMPONENTS filesystem system) # Set your boost version
```
3.Create an build directory, go into it and then run `cmake` in it, use the following command.

```bash
cmake --build . --config Release
```
*Note: If there is nothing wrong, you will get the cpppls.exe or cpppls(bin). Congratulation! Just get it from the path: **build/Release/***

##### 3.Compile the symbol_scanner (optional)
*Note: If you want to enjoy the features of **Symbols in File** and **Go to definition**, symbol_scanner is necessary.*

1.Just go into the path: **tools/symbol_scanner/**, use the following commands to compile the symbol_scanner.

```bash
flex symbol_scanner.l           # create the lex.yy.c or other xxx.c you like
gcc lex.yy.c -o symbol_scanner  # use the above result(lex.yy.c or other xxx.c)
```

*Note: If there is nothing wrong, you will get the symbol_scanner.exe or symbol_scanner(bin). Congratulation! Just get it from the path: **tools/symbol_scanner***

## Usage

There are several params you need to configure for the use of the cpppls as following:

| name | description | example | default |
| ---- | ----------- | ------- | ---- |
| cpppls.perl_exe | Path to the perl binary to use for syntax checking            | "d:/Users/cpppls.exe" or "/home/cpppls" | "" |
| cpppls.perl_incs | Paths to add to @INC，give the file path where your perl lib files live in | ["c:/Strawberry/perl/lib/", "c:/test/"] or ["/usr/share/perl5/lib/", "/home/lib/"] | [] |
| cpppls.symbol_scanner.enabled | Enable symbol scanner | true | false |
| cpppls.symbol_scanner.path | Path to the symbol scanner | "d:/Users/tools/" or "/home/tools/" | "" |

Because I am a neovimer, and I always use the coc.nvim plugin. So here I can give the example for how to configure it with neovim and coc.nvim plugin.

### Neovim

##### coc.nvim

1.Use `:CocConfig` in nvim command mode and get the coc-settings.json.

2.Configure your coc-settings.json. Here is the configuration example of coc-settings.json.

```json
{
	"languageserver": {
        "cpppls": {
            "command": "d:/Users/User/AJin/cpppls.exe",
            "args": ["--stdio"],
            "filetypes": ["perl"],
            "trace.server": "verbose",
            "settings": {
                "cpppls": {
                    "perl_exe": "perl",
                    "perl_incs": [
                        "c:/Strawberry/perl/lib/",
                        "c:/Strawberry/perl/site/lib/",
                        "c:/Strawberry/perl/vendor/lib/"
                    ],
                    "symbol_scanner": {
                        "enabled": true,
                        "path": "d:/Users/User/AJin/tools/symbol_scanner/"
                    }
                }
            }
        }
    }
}
```
