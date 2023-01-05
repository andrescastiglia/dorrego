#include <boost/asio.hpp>
#include <iostream>

#include "main.h"
#include "listener.h"

using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::port_type;
using boost::asio::ip::tcp;

Listener::Listener(port_type _port, std::function<void(Order &_order, Trade &_trade)> _callback) : port(_port), callback(_callback)
{
}

awaitable<void> Listener::perform()
{
    auto executor = co_await boost::asio::this_coro::executor;

    tcp::acceptor acceptor(executor, {tcp::v4(), this->port});
    while (acceptor.is_open())
    {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        boost::asio::co_spawn(executor, this->accept(std::move(socket)), detached);
    }
}

awaitable<void> Listener::accept(tcp::socket _socket)
{
    try
    {
        while (_socket.is_open())
        {
            co_await this->receive(_socket);
        }
    }
    catch (std::exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}

awaitable<void> Listener::receive(tcp::socket &_socket)
{
    Order order;

    try
    {
        co_await boost::asio::async_read(_socket, boost::asio::buffer(&order, ORDER_SIZE), use_awaitable);
    }
    catch (boost::system::system_error &_ex)
    {
        if (_ex.code() != boost::asio::error::eof &&
            _ex.code() != boost::asio::error::connection_reset &&
            _ex.code() != boost::asio::error::connection_aborted)
        {
            std::cerr << _ex.what() << std::endl
                      << std::flush;
        }
        _socket.close();
        co_return;
    }

    Trade trade{};
    this->callback(order, trade);

    try
    {
        co_await boost::asio::async_write(_socket, boost::asio::buffer(&trade, TRADE_SIZE), use_awaitable);
    }
    catch (boost::system::system_error &_ex)
    {
        if (_ex.code() != boost::asio::error::eof &&
            _ex.code() != boost::asio::error::connection_reset &&
            _ex.code() != boost::asio::error::connection_aborted)
        {
            std::cerr << _ex.what() << std::endl
                      << std::flush;
        }
        _socket.close();
    }
}