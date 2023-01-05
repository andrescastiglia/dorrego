#pragma once

using boost::asio::awaitable;
using boost::asio::ip::tcp;
using boost::asio::ip::port_type;

class Listener
{
private:
    awaitable<void> accept(tcp::socket _socket);
    awaitable<void> receive(tcp::socket &_socket);

    port_type port;
    std::function<void(Order &_order, Trade &_trade)> callback;

public:
    Listener(port_type _port, std::function<void(Order &_order, Trade &_trade)> _callback);
    virtual ~Listener() = default;

    awaitable<void> perform();
};