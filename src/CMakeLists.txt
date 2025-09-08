# UDP socket wrapper
add_library(udp_socket_static STATIC UDPSocket.cpp)
add_library(udp_socket_shared SHARED UDPSocket.cpp)

# Blocklist
add_library(blocklist_static STATIC DNSBlockList.cpp)
add_library(blocklist_shared SHARED DNSBlockList.cpp)

# Parser
add_library(parser_static STATIC DNSParser.cpp)
add_library(parser_shared SHARED DNSParser.cpp)

# Logger
add_library(logger_static STATIC Logger.cpp)
add_library(logger_shared SHARED Logger.cpp)

# AdBlocker core
add_library(adblocker_static STATIC DNSAdBlocker.cpp)
add_library(adblocker_shared SHARED DNSAdBlocker.cpp)

# Link dependencies
target_link_libraries(adblocker_static
        PRIVATE blocklist_static parser_static
        PUBLIC udp_socket_static
)

target_link_libraries(adblocker_shared
        PRIVATE blocklist_shared parser_shared
        PUBLIC udp_socket_shared
)

# DNS server executable
add_executable(dns_server main.cpp)

# Link against adblocker (udp_socket comes transitively)
target_link_libraries(dns_server PRIVATE adblocker_static)
