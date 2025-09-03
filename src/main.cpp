
#include <iostream>

#include <iomanip>   // for std::setw, std::setfill

#include "DNSAdBlocker.hpp"

using namespace boost::asio::ip;


void hex_dump(const uint8_t* data, std::size_t length)
{
    for (std::size_t i = 0; i < length; i++)
    {
        if (i % 16 == 0) std::cout << std::endl << std::setw(4) << std::setfill('0') << i << ": ";
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::dec << std::endl; // back to decimal
}

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

    adblocker.Run();
    io.run();

    return 0;
}
