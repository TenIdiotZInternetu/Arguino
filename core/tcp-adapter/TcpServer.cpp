//
// Created by touster on 10. 10. 2025.
//

#include "TcpServer.hpp"

using namespace boost;

namespace arguino::tcp {
    template <ConnectionHandler THandler>
    TcpServer<THandler>::TcpServer(uint16_t port) :
        _socket(_io_context),
        _endpoint(asio::ip::tcp::v4(), port),
        _acceptor(_io_context, _endpoint),
        _port(port) {}

    template <ConnectionHandler THandler>
    void TcpServer<THandler>::launch() {
        _acceptor.listen();
        start_accepting();
    }

    template <ConnectionHandler THandler>
    void TcpServer<THandler>::start_accepting() {
        std::shared_ptr<THandler> handler = THandler::create();

        _acceptor.async_accept(handler->socket(), [this, handler](auto error) {
            if (!error) {
                handler->handle();
            } else {
                //TODO log
            }
            start_accepting();
        });
    }
} // tcp
// arguino