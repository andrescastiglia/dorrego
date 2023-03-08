#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include "main.h"
#include "orderbook.h"

#include <boost/test/unit_test.hpp>

using namespace dorrego;

struct OrderbookTest
{
  OrderbookTest()
  {
    Orderbook::destroy();
  }
  virtual ~OrderbookTest()
  {
    Orderbook::destroy();
  }
};

BOOST_FIXTURE_TEST_SUITE(OrderbookTestSuite, OrderbookTest)

BOOST_AUTO_TEST_CASE(add_order)
{
  Orderbook orderbook;

  Order order(1, 1, 1, 100, 1, Side::Buy);
  BOOST_TEST(orderbook.add(order), "Failed to add to Orderbook");
}

BOOST_AUTO_TEST_CASE(add_order_duplicated)
{
  Orderbook orderbook;

  Order order(2, 2, 1, 100, 1, Side::Buy);
  BOOST_TEST(orderbook.add(order), "Failed to add to Orderbook");

  BOOST_TEST(!orderbook.add(order), "Failed to detect duplicated order");
}

BOOST_AUTO_TEST_CASE(remove_order)
{
  Orderbook orderbook;

  Order order(3, 3, 1, 100, 1, Side::Buy);
  BOOST_TEST(orderbook.add(order), "Failed to add to Orderbook");

  Order out;
  BOOST_TEST(orderbook.take(3, 1, Side::Buy, out), "Failed to remove");
}

BOOST_AUTO_TEST_CASE(not_remove_order)
{
  Orderbook orderbook;

  Order order(4, 4, 1, 100, 1, Side::Buy);
  BOOST_TEST(orderbook.add(order));

  Order out;
  BOOST_TEST(!orderbook.take(5, 1, Side::Buy, out), "Unexpected remove");
  BOOST_TEST(!orderbook.take(4, 1, Side::Sell, out), "Unexpected remove");
  BOOST_TEST(!orderbook.take(5, 1, Side::Sell, out), "Unexpected remove");
}

BOOST_AUTO_TEST_CASE(match)
{
  Orderbook orderbook;

  Order order1(6, 6, 1, 100, 1, Side::Buy);
  BOOST_TEST(orderbook.add(order1), "Failed to add to Orderbook");

  Order order2(7, 7, 1, 100, 1, Side::Sell);
  ID id_matched;
  BOOST_TEST(orderbook.match(order2, id_matched), "Failed to match");

  BOOST_TEST(order1.id == id_matched, "Wrong match");
}

BOOST_AUTO_TEST_SUITE_END()