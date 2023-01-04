
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <cstring>
#include "saver.h"
#include "listener.h"
#include "in_channel.h"

using boost::asio::detached;

InChannel::InChannel() : listener(Listener())
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
    catch (std::exception &e)
    {
        std::printf("Exception: %s\n", e.what());
    }
}