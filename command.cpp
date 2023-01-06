#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>

#include "main.h"
#include "orderbook.h"
#include "command.h"

using namespace boost::interprocess;

Command::Command(Orderbook &_orderbook, Order &_order, Trade &_trade) : orderbook(_orderbook), order(_order), trade(_trade)
{
}

void Command::execute()
{
    unsigned long id_matched;
    auto opposite = this->order.side == Side::Buy ? Side::Sell : Side::Buy;

    try
    {
        scoped_lock<interprocess_mutex> lock(this->mutex);

        if (this->orderbook.match(this->order, id_matched))
        {
            if (this->orderbook.remove(id_matched, opposite))
            {
                this->trade.id_buyer = this->order.side == Side::Buy ? this->order.id : id_matched;
                this->trade.id_seller = this->order.side == Side::Buy ? id_matched : this->order.id;

                return;
            }
        }

        this->orderbook.add(this->order);
    }
    catch (interprocess_exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}
