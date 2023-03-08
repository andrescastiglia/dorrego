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

using namespace dorrego;

Orderbook::Orderbook()
{
    this->segment = managed_shared_memory(open_or_create, ORDERBOOK_SEGMENT, ORDERBOOK_SIZE);

    auto allocator = this->segment.get_allocator<Order>();

    for (auto currency = 0; currency < MAX_CURRENCY; currency++)
    {
        this->orderbook[currency][Side::Buy] = this->segment.find_or_construct<OrderbookContainer>(PURCHASES_CONTAINER)(allocator);
        this->orderbook[currency][Side::Sell] = this->segment.find_or_construct<OrderbookContainer>(SALES_CONTAINER)(allocator);
    }
}

bool Orderbook::destroy()
{
    return shared_memory_object::remove(ORDERBOOK_SEGMENT);
}

bool Orderbook::add(Order &_order)
{
    auto result = this->orderbook[_order.currency][_order.side]->insert(_order);

    return result.second;
}

bool Orderbook::take(ID _id, CURRENCY _currency, Side _side, Order &_order)
{
    auto item = this->orderbook[_currency][_side]->get<by_id>().find(_id);
    if (item == this->orderbook[_currency][_side]->get<by_id>().end())
        return false;

    _order = *this->orderbook[_currency][_side]->erase(item);

    return true;
}

bool Orderbook::match(Order &_order, Order &_matched)
{
    if (_order.side == Side::Buy)
    {
        auto sale = this->orderbook[_order.currency][Side::Sell]->get<by_price>().begin();
        if (sale != this->orderbook[_order.currency][Side::Sell]->get<by_price>().end())
        {
            if (_order.price >= sale->price)
            {
                _matched = *sale;
                return true;
            }
        }
    }
    else
    {
        auto purchase = this->orderbook[_order.currency][Side::Buy]->get<by_price>().find(_order.price);
        if (purchase != this->orderbook[_order.currency][Side::Buy]->get<by_price>().end())
        {
            _matched = *purchase;
            return true;
        }

        purchase = this->orderbook[_order.currency][Side::Buy]->get<by_price>().upper_bound(_order.price);
        if (purchase != this->orderbook[_order.currency][Side::Buy]->get<by_price>().end())
        {
            _matched = *purchase;
            return true;
        }
    }
    return false;
}