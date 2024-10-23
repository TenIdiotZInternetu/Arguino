//
// Created by TIZI on 05/10/2024.
//

#include <iostream>
#include "boost/asio.hpp"
#include "TCPConnection.hpp"

TCPConnection::TCPConnection() {
    boost::asio::io_context ioc;
    std::cout << "i live";
}
