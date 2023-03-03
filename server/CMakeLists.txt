# 0.Set minimum required cmake_version
cmake_minimum_required(VERSION 3.5)

# 1.Set project name£¬project_version and project_programming_language
project(cpppls VERSION 0.1 LANGUAGES CXX)

# 2.Set std=c++11 required
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)

# 3.Boost config (Windows)
if (WIN32)
    set(BOOST_ROOT C:/Boost)
    set(Boost_LIBRARY_DIRS C:/Boost/lib)
endif (WIN32)

# 3.1 Boost config (Linux)
if (UNIX)
    set(BOOST_ROOT /home/ajin/Desktop/ajlib/Boost)
    set(Boost_LIBRARY_DIRS /home/ajin/Desktop/ajlib/Boost/lib)
endif (UNIX)

find_package(Boost 1.80 REQUIRED COMPONENTS filesystem system)
if (Boost_FOUND)
    message ("Boost found.")
else ()
    message (FATAL_ERROR "Cannot find Boost!")
endif (Boost_FOUND)

# 4.Set source files (*.cpp)
set(SOURCES
    factory/lifecycle.cpp
    factory/textdocument.cpp
    parser/uri.cpp
    handler/client/initialize.cpp
    handler/client/initialized.cpp
    handler/client/textdocument/didopen.cpp
    handler/client/textdocument/documentsymbol.cpp
    handler/server/request/workspace/configuration.cpp
    handler/server/notification/textdocument/publishdiagnostics.cpp
    ./main.cpp
    ./server.cpp
)

# 5.Target config
add_executable(${PROJECT_NAME} ${SOURCES})

# 5.1 #include config
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/factory
        ${PROJECT_SOURCE_DIR}/fmt
        ${PROJECT_SOURCE_DIR}/nlohmann
        ${PROJECT_SOURCE_DIR}/parser
        ${PROJECT_SOURCE_DIR}/handler
        ${PROJECT_SOURCE_DIR}/handler/client
        ${PROJECT_SOURCE_DIR}/handler/client/textdocument
        ${PROJECT_SOURCE_DIR}/handler/server/notification/textdocument
        ${PROJECT_SOURCE_DIR}/handler/server/request/workspace
)

# 5.1 link config
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Boost::filesystem
)