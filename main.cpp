#include <boost/asio.hpp>
#include <boost/mpi.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>

#include "main.h"
#include "balances.h"
#include "orderbook.h"
#include "command.h"
#include "listener.h"
#include "context.h"

namespace mpi = boost::mpi;
using namespace boost::asio;
using namespace dorrego;

int main(int argc, char *argv[])
{
    mpi::environment env(argc, argv);
    mpi::communicator world;

    if (world.size() == 1)
    {
        auto result = Orderbook::destroy() && Balances::destroy();

        std::cout << "Clean shared memory. " << result << std::endl
                  << std::flush;
    }
    else if (world.rank() == 0)
    {
        std::cout << "Server: " << world.rank() << " of " << world.size() << "." << std::endl
                  << std::flush;

        ip::port_type port = 1000;
        Context<Orderbook, Order, Trade> context(port);
        context.run();
    }
    else if (world.rank() == 1)
    {
        std::cout << "Server: " << world.rank() << " of " << world.size() << "." << std::endl
                  << std::flush;

        ip::port_type port = 1001;
        Context<Balances, Transfer, Balance> context(port);
        context.run();
    }
    else if (world.rank() < 4)
    {
        sleep(2);
        std::cout << "Client: " << world.rank() << " of " << world.size() << "." << std::endl
                  << std::flush;

        io_service service;
        ip::tcp::socket socket(service);
        ip::port_type port = 1000;
        socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), port));

        {
            ID id = 1 + world.rank();
            ID account = 10 + world.rank();
            CURRENCY currency = 0;
            AMOUNT price = 100 + world.rank();
            AMOUNT amount = 5;
            Side side = id % 2 == 0 ? Side::Buy : Side::Sell;
            Order order{id, account, currency, price, amount, side};

            write(socket, boost::asio::buffer(&order, ORDER_SIZE), transfer_exactly(ORDER_SIZE));
        }
        {
            Trade trade;
            read(socket, boost::asio::buffer(&trade, TRADE_SIZE), transfer_exactly(TRADE_SIZE));

            std::cout << "Client: seller " << trade.order_id_seller << " - buyer " << trade.order_id_buyer << "." << std::endl
                      << std::flush;
        }
    }
    else if (world.rank() < 6)
    {
        sleep(2);
        std::cout << "Client: " << world.rank() << " of " << world.size() << "." << std::endl
                  << std::flush;

        io_service service;
        ip::tcp::socket socket(service);
        ip::port_type port = 1001;
        socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), port));
        {
            ID account = 1 + world.rank();
            CURRENCY currency = 0;
            AMOUNT amount = 100 + world.rank();
            Operation operation = Operation::Add;
            Transfer transfer{account, currency, amount, operation};
            write(socket, boost::asio::buffer(&transfer, TRANSFER_SIZE), transfer_exactly(TRANSFER_SIZE));
        }
        {
            Balance balance;
            read(socket, boost::asio::buffer(&balance, BALANCE_SIZE), transfer_exactly(BALANCE_SIZE));

            std::cout << "Client: balance " << balance.amount << "." << std::endl
                      << std::flush;
        }
    }
    else
    {
        std::cerr << "Too much: " << world.rank() << "." << std::endl
                  << std::flush;
    }

    return EXIT_SUCCESS;
}