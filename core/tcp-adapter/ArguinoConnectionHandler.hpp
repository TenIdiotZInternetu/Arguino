//
// Created by touster on 12. 10. 2025.
//

#ifndef ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP
#define ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP

#include <boost/asio/ip/tcp.hpp>

class ArguinoConnectionHandler :
   public std::enable_shared_from_this<ArguinoConnectionHandler> {
public:
    using pointer_t = std::shared_ptr<ArguinoConnectionHandler>;

    ArguinoConnectionHandler();

    static pointer_t create();
    void handle();
    boost::asio::ip::tcp::socket& socket();
private:

};


#endif //ARGUINO_CORE_ARGUINOMESSAGEHANDLER_HPP