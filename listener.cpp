#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

#include <cstring>
#include "saver.h"
#include "listener.h"

using boost::asio::detached;
using boost::asio::use_awaitable;

#define BUFFER_LEN 1024

Listener::Listener() : saver(Saver())
{
}

awaitable<void> Listener::perform()
{
    auto executor = co_await boost::asio::this_coro::executor;

    tcp::acceptor acceptor(executor, {tcp::v4(), 5555});
    for (;;)
    {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        boost::asio::co_spawn(executor, this->accept(std::move(socket)), detached);
    }
}

awaitable<void> Listener::accept(tcp::socket _socket)
{
    try
    {
        for (;;)
        {
            co_await this->exec(_socket);
        }
    }
    catch (std::exception &e)
    {
        std::printf("Exception: %s\n", e.what());
    }
}

awaitable<void> Listener::exec(tcp::socket &_socket)
{
    char in_buffer[BUFFER_LEN];
    char out_buffer[BUFFER_LEN];
    std::size_t in_len, out_len;

    in_len = co_await _socket.async_read_some(boost::asio::buffer(in_buffer), use_awaitable);

    this->saver.perform(in_buffer, in_len, out_buffer, out_len);

    co_await boost::asio::async_write(_socket, boost::asio::buffer(out_buffer, out_len), use_awaitable);
}