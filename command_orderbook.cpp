#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>

#include "main.h"
#include "orderbook.h"
#include "command.h"

using namespace dorrego;
using namespace boost::interprocess;

template <>
void Command<Orderbook, Order, Trade>::execute()
{
    ID order_id_matched;
    auto opposite = this->request.side == Side::Buy ? Side::Sell : Side::Buy;
    auto currency = this->request.currency;

    try
    {
        scoped_lock<named_mutex> lock(this->mutex);

        if (this->book.match(this->request, order_id_matched))
        {
            if (this->book.remove(order_id_matched, currency, opposite))
            {
                this->response.order_id_buyer = this->request.side == Side::Buy ? this->request.id : order_id_matched;
                this->response.order_id_seller = this->request.side == Side::Buy ? order_id_matched : this->request.id;

                return;
            }
        }

        this->book.add(this->request);
    }
    catch (interprocess_exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}