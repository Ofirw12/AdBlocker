
#include <iostream>

#include "DNSAdBlocker.hpp"

using namespace boost::asio::ip;

int main()
{
    boost::asio::io_context io;
    std::string listen_ip = "127.0.0.1";
    unsigned short listen_port = 5300;
    std::string dns_ip = "8.8.8.8";
    unsigned short dns_port = 53;
    std::string block_list_path = "blocklist.txt";
    adblocker::DNSAdBlocker adblocker(io, listen_ip, listen_port,
            dns_ip, dns_port, block_list_path);
    
    std::cout << "Start listening" << std::endl;
    adblocker.Run();
    io.run();

    return 0;
}
