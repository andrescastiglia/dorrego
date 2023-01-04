#pragma once

using boost::asio::awaitable;
using boost::asio::ip::tcp;

class Listener
{
public:
    Listener();
    awaitable<void> perform();

private:
    awaitable<void> accept(tcp::socket _socket);
    awaitable<void> exec(tcp::socket &_socket);
    Saver saver;
};