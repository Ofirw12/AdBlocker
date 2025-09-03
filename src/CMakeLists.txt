cmake_minimum_required(VERSION 4.0)

project(adblocker)
add_library(adblocker_static STATIC DNSAdBlocker.cpp)
add_library(adblocker_shared SHARED DNSAdBlocker.cpp)

project(blocklist)
add_library(blocklist_static STATIC DNSBlockList.cpp)
add_library(blocklist_shared SHARED DNSBlockList.cpp)

project(parser)
add_library(parser_static STATIC DNSParser.cpp)
add_library(parser_shared SHARED DNSParser.cpp)

target_link_libraries(adblocker_static blocklist_static parser_static)
target_link_libraries(adblocker_shared blocklist_shared parser_shared)

project(dnsserver)
add_executable(dns_server main.cpp)
target_link_libraries(dns_server adblocker_shared)