#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>

#include "main.h"
#include "orderbook.h"
#include "command.h"

#include <boost/test/unit_test.hpp>

using namespace dorrego;

struct CommandTest
{
    CommandTest()
    {
        Orderbook::destroy();
    }
    virtual ~CommandTest()
    {
        Orderbook::destroy();
    }
};

BOOST_FIXTURE_TEST_SUITE(CommandTestSuite, CommandTest)

BOOST_AUTO_TEST_CASE(simple_order)
{
    Orderbook orderbook;

    Trade trade{};
    Order order{1, 1, 1, 900, 10, Side::Sell};
    Command command(orderbook, order, trade);

    command.execute();

    BOOST_TEST(trade.order_id_buyer == 0, "Unexpected trade");
    BOOST_TEST(trade.order_id_seller == 0, "Unexpected trade");
}

BOOST_AUTO_TEST_CASE(buy_sell_equal_trade)
{
    Orderbook orderbook;

    Trade trade{};
    {
        Order order{2, 2, 1, 800, 10, Side::Buy};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_buyer == 0, "Unexpected trade");
    BOOST_TEST(trade.order_id_seller == 0, "Unexpected trade");

    {
        Order order{3, 3, 3, 800, 10, Side::Sell};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_buyer == 2, "Failed to trade");
    BOOST_TEST(trade.order_id_seller == 3, "Failed to trade");
}

BOOST_AUTO_TEST_CASE(sell_buy_equal_trade)
{
    Orderbook orderbook;

    Trade trade{};
    {
        Order order{4, 4, 1, 700, 10, Side::Sell};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_buyer == 0, "Unexpected trade");
    BOOST_TEST(trade.order_id_seller == 0, "Unexpected trade");

    {
        Order order{5, 5, 1, 700, 10, Side::Buy};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_seller == 4, "Failed to trade");
    BOOST_TEST(trade.order_id_buyer == 5, "Failed to trade");
}

BOOST_AUTO_TEST_CASE(buy_sell_lower_trade)
{
    Orderbook orderbook;

    Trade trade{};
    {
        Order order{6, 6, 1, 600, 10, Side::Buy};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_buyer == 0, "Unexpected trade");
    BOOST_TEST(trade.order_id_seller == 0, "Unexpected trade");

    {
        Order order{7, 7, 1, 590, 10, Side::Sell};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_buyer == 6, "Failed to trade");
    BOOST_TEST(trade.order_id_seller == 7, "Failed to trade");
}

BOOST_AUTO_TEST_CASE(sell_buy_lower_trade)
{
    Orderbook orderbook;

    Trade trade{};
    {
        Order order{8, 8, 1, 450, 10, Side::Sell};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_buyer == 0, "Unexpected trade");
    BOOST_TEST(trade.order_id_seller == 0, "Unexpected trade");

    {
        Order order{9, 9, 1, 500, 10, Side::Buy};
        Command command(orderbook, order, trade);
        command.execute();
    }
    BOOST_TEST(trade.order_id_seller == 8, "Failed to trade");
    BOOST_TEST(trade.order_id_buyer == 9, "Failed to trade");
}

BOOST_AUTO_TEST_SUITE_END()