#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include "main.h"
#include "orderbook.h"

#include <boost/test/unit_test.hpp>

struct OrderbookTest
{
  OrderbookTest()
  {
    BOOST_TEST_MESSAGE("Flush");
    Orderbook::destroy();
  }
};

BOOST_FIXTURE_TEST_SUITE(OrderbookTestSuite, OrderbookTest)

BOOST_AUTO_TEST_CASE(add_order)
{
  Orderbook orderbook;

  Order order{1, 100, 1, Side::Buy};
  BOOST_TEST(orderbook.add(order), "Failed to add to Orderbook");
}

BOOST_AUTO_TEST_CASE(add_order_duplicated)
{
  Orderbook orderbook;

  Order order{2, 100, 1, Side::Buy};
  BOOST_TEST(orderbook.add(order), "Failed to add to Orderbook");

  BOOST_TEST(!orderbook.add(order), "Failed to detect duplicated order");
}

BOOST_AUTO_TEST_CASE(remove_order)
{
  Orderbook orderbook;

  Order order{3, 100, 1, Side::Buy};
  BOOST_TEST(orderbook.add(order), "Failed to add to Orderbook");

  BOOST_TEST(orderbook.remove(3, Side::Buy), "Failed to remove");
}

BOOST_AUTO_TEST_CASE(not_remove_order)
{
  Orderbook orderbook;

  Order order{4, 100, 1, Side::Buy};
  BOOST_TEST(orderbook.add(order));

  BOOST_TEST(!orderbook.remove(5, Side::Buy), "Unexpected remove");
  BOOST_TEST(!orderbook.remove(4, Side::Sell), "Unexpected remove");
  BOOST_TEST(!orderbook.remove(5, Side::Sell), "Unexpected remove");
}

BOOST_AUTO_TEST_CASE(match)
{
  Orderbook orderbook;

  Order order1{6, 100, 1, Side::Buy};
  BOOST_TEST(orderbook.add(order1), "Failed to add to Orderbook");

  Order order2{7, 100, 1, Side::Sell};
  unsigned long id_matched;
  BOOST_TEST(orderbook.match(order2, id_matched), "Failed to match");

  BOOST_TEST(order1.id == id_matched, "Wrong match");
}

BOOST_AUTO_TEST_SUITE_END()