#include <iostream>

#include <iomanip>   // for std::setw, std::setfill
#include <utility>
#include <boost/asio.hpp>

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
    udp::socket sock(io, udp::endpoint(udp::v4(), 5300)); // 53 requires root, 5353 don't
    std::array<uint8_t, 1500> buffer{};
    udp::endpoint ep;


    io.run();

    // std::cout << "Hello, World!" << std::endl;
    return 0;
}