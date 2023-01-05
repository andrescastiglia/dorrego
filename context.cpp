#include <boost/asio.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>

#include "main.h"
#include "orderbook.h"
#include "command.h"
#include "listener.h"
#include "context.h"

using boost::asio::detached;
using boost::asio::ip::port_type;

Context::Context(int _rank, port_type _port) : rank(_rank), port(_port),
                                               orderbook(Orderbook()),
                                               listener(Listener(_port, [&](Order &_order, Trade &_trade)
                                                                 { return this->callback(_order, _trade); }))
{
}

void Context::run()
{
    try
    {
        boost::asio::io_context io_context(10);
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

        signals.async_wait([&](auto, auto)
                           { io_context.stop(); });

        boost::asio::co_spawn(io_context, this->listener.perform(), detached);

        io_context.run();
    }
    catch (std::exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}

void Context::callback(Order &_order, Trade &_trade)
{
    Command command(this->orderbook, _order, _trade);

    command.execute();
}