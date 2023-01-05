#pragma once

using boost::asio::ip::port_type;

class Context
{
private:
    Orderbook orderbook;
    Listener listener;

public:
    Context(int _rank, port_type _port);
    virtual ~Context() = default;

    void run();
    void callback(Order &_order, Trade &_trade);

    const int rank;
    const port_type port;
};