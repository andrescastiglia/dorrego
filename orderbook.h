#pragma once

using namespace boost::interprocess;
using namespace boost::multi_index;

typedef boost::multi_index_container<
    Order,
    indexed_by<
        ordered_unique<tag<struct by_id>, member<Order, unsigned long, &Order::id>>,
        ordered_non_unique<tag<struct by_price>, member<Order, unsigned long, &Order::price>>>,
    allocator<Order, managed_shared_memory::segment_manager>>
    OrderbookContainer;

class Orderbook
{
private:
    managed_shared_memory segment;
    OrderbookContainer *orderbook[2];

public:
    Orderbook();
    virtual ~Orderbook() = default;

    static bool destroy();

    bool add(Order &_order);
    bool remove(unsigned long _id, Side _side);
    bool match(Order &_order, unsigned long &_id);
};
