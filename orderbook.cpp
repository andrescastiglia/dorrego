#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include "main.h"
#include "orderbook.h"

#define ORDERBOOK_SEGMENT "OrderbookSegment"
#define PURCHASES_CONTAINER "PurchasesContainer"
#define SALES_CONTAINER "SalesContainer"

Orderbook::Orderbook()
{
    this->segment = managed_shared_memory(open_or_create, ORDERBOOK_SEGMENT, ORDERBOOK_SIZE);

    auto allocator = this->segment.get_allocator<Order>();

    this->orderbook[Side::Buy] = this->segment.find_or_construct<OrderbookContainer>(PURCHASES_CONTAINER)(allocator);
    this->orderbook[Side::Sell] = this->segment.find_or_construct<OrderbookContainer>(SALES_CONTAINER)(allocator);
}

bool Orderbook::destroy()
{
    return shared_memory_object::remove(ORDERBOOK_SEGMENT);
}

bool Orderbook::add(Order &_order)
{
    auto result = this->orderbook[_order.side]->insert(_order);

    return result.second;
}

bool Orderbook::remove(unsigned long _id, Side _side)
{
    auto item = this->orderbook[_side]->get<by_id>().find(_id);
    if (item == this->orderbook[_side]->get<by_id>().end())
    {
        return false;
    }

    this->orderbook[_side]->erase(item);

    return true;
}

bool Orderbook::match(Order &_order, unsigned long &_id)
{
    if (_order.side == Side::Buy)
    {
        auto sale = this->orderbook[Side::Sell]->get<by_price>().begin();
        if (sale != this->orderbook[Side::Sell]->get<by_price>().end())
        {
            if (_order.price >= sale->price)
            {
                _id = sale->id;
                return true;
            }
        }
    }
    else
    {
        auto purchase = this->orderbook[Side::Buy]->get<by_price>().find(_order.price);
        if (purchase != this->orderbook[Side::Buy]->get<by_price>().end())
        {
            _id = purchase->id;
            return true;
        }

        purchase = this->orderbook[Side::Buy]->get<by_price>().upper_bound(_order.price);
        if (purchase != this->orderbook[Side::Buy]->get<by_price>().end())
        {
            _id = purchase->id;
            return true;
        }
    }
    return false;
}