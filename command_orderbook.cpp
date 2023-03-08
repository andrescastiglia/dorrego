#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>
#include <algorithm>

#include "main.h"
#include "orderbook.h"
#include "balances.h"
#include "command.h"

#define MUTEX_NAME "DOBM"

#define PHASE_NONE 0
#define PHASE_REMOVED 1
#define PHASE_ADDED 2

using namespace dorrego;
using namespace boost::interprocess;

template <>
Command<Orderbook, Order, Trade>::Command(Orderbook &_book, Order &_request, Trade &_response)
    : book(_book), request(_request), response(_response), succeeded(false), phase(PHASE_NONE)
{
    this->mutex_name = MUTEX_NAME;
}

template <>
Command<Orderbook, Order, Trade>::~Command()
{
}

template <>
void Command<Orderbook, Order, Trade>::execute()
{
    Order order_matched;
    auto opposite = this->request.side == Side::Buy ? Side::Sell : Side::Buy;
    auto currency = this->request.currency;

    try
    {
        named_mutex mutex(open_or_create, this->mutex_name);

        scoped_lock<named_mutex> lock(mutex);

        if (this->book.match(this->request, order_matched))
        {
            auto buyer = this->request.side == Side::Buy ? this->request : order_matched;
            auto seller = this->request.side == Side::Sell ? this->request : order_matched;
            auto amount = std::min(buyer.amount, seller.amount);

            /*
            Transfer debit1 {buyer.account, CURRENCY_BASE, amount, Operation::Substract};
            Transfer debit2 {seller.account, currency, amount, Operation::Substract};

            Transfer credit1 {buyer.account, currency, amount, Operation::Add};
            Transfer credit2 {seller.account, CURRENCY_BASE, amount, Operation::Add};
            */

            if (this->book.take(order_matched.id, currency, opposite))
            {
                this->response.order_id_buyer = buyer.id;
                this->response.order_id_seller = seller.id;
                this->response.amount = amount;

                this->phase = PHASE_REMOVED;
                this->succeeded = true;
            }
        }
        else if ( this->book.add(this->request) )
        {
            this->phase = PHASE_ADDED;
            this->succeeded = true;
        }
    }
    catch (interprocess_exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}

template <>
void Command<Orderbook, Order, Trade>::rollback()
{
    if (!this->succeeded)
        return;

    switch ( this->phase )
    {
        case PHASE_ADDED: 
            this->book.remove();
            break;

        case PHASE_REMOVED:
            this->book.add()
            break;

        default:
            break;
    }
}