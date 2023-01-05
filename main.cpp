#include <boost/asio.hpp>
#include <boost/mpi.hpp>
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

namespace mpi = boost::mpi;
using namespace boost::asio;

int main(int argc, char *argv[])
{
    mpi::environment env(argc, argv);
    mpi::communicator world;

    if (world.size() == 1)
    {
        auto result = Orderbook::destroy();

        std::cout << "Clean shared memory. " << result << std::endl
                  << std::flush;
    }
    else if (world.rank() < 3)
    {
        std::cout << "Server: " << world.rank() << " of " << world.size() << "." << std::endl
                  << std::flush;

        ip::port_type port = 1000 + world.rank();

        Context context(world.rank(), port);
        context.run();
    }
    else if (world.rank() < 6)
    {
        sleep(2);
        std::cout << "Client: " << world.rank() << " of " << world.size() << "." << std::endl
                  << std::flush;

        io_service service;
        ip::tcp::socket socket(service);
        ip::port_type port = 1000 + world.rank() - 3;
        socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), port));

        unsigned long id = 1 + world.rank();
        Side side = id % 2 == 0 ? Side::Buy : Side::Sell;
        unsigned long price = 100 + world.rank();
        Order order{id, price, 5, side};
        write(socket, boost::asio::buffer(&order, ORDER_SIZE), transfer_exactly(ORDER_SIZE));

        Trade trade;
        read(socket, boost::asio::buffer(&trade, TRADE_SIZE), transfer_exactly(TRADE_SIZE));

        std::cout << "Client: seller " << trade.id_seller << " - buyer " << trade.id_buyer << "." << std::endl
                  << std::flush;
    }
    else
    {
        std::cerr << "Too much: " << world.rank() << "." << std::endl
                  << std::flush;
    }

    return EXIT_SUCCESS;
}