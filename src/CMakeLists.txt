add_library(adblocker_static STATIC DNSAdBlocker.cpp)
add_library(adblocker_shared SHARED DNSAdBlocker.cpp)


add_library(blocklist_static STATIC DNSBlockList.cpp)
add_library(blocklist_shared SHARED DNSBlockList.cpp)

add_library(parser_static STATIC DNSParser.cpp)
add_library(parser_shared SHARED DNSParser.cpp)

add_library(logger_static STATIC Logger.cpp)
add_library(logger_shared SHARED Logger.cpp)

target_link_libraries(adblocker_static PRIVATE Boost::system Boost::filesystem)
target_link_libraries(adblocker_shared PRIVATE Boost::system Boost::filesystem)

target_link_libraries(adblocker_static PRIVATE blocklist_static parser_static)
target_link_libraries(adblocker_shared PRIVATE blocklist_shared parser_shared)

add_executable(dns_server main.cpp)
target_link_libraries(dns_server PRIVATE adblocker_shared)

