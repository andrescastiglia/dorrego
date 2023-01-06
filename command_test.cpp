#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>

#include "main.h"
#include "orderbook.h"
#include "command.h"

#include <boost/test/unit_test.hpp>

struct CommandTest
{
    CommandTest()
    {
        BOOST_TEST_MESSAGE("Flush");
        Orderbook::destroy();
    }
};

BOOST_FIXTURE_TEST_SUITE(CommandTestSuite, CommandTest)

BOOST_AUTO_TEST_CASE(simple_order)
{
    Orderbook orderbook;

    Trade trade{};
    Order order{1, 100, 10, Side::Buy};
    Command command(orderbook, order, trade);

    command.execute();

    BOOST_TEST(trade.id_buyer == 0, "Unexpected trade");
    BOOST_TEST(trade.id_seller == 0, "Unexpected trade");
}

BOOST_AUTO_TEST_SUITE_END()