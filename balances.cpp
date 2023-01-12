#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include "main.h"
#include "balances.h"

#define BALANCES_SEGMENT "BalancesSegment"
#define BALANCES_CONTAINER "BalancesContainer"

using namespace dorrego;

Balances::Balances()
{
    this->segment = managed_shared_memory(open_or_create, BALANCES_SEGMENT, BALANCES_SIZE);

    auto allocator = this->segment.get_allocator<Balance>();

    this->balances = this->segment.find_or_construct<BalancesContainer>(BALANCES_CONTAINER)(allocator);
}

bool Balances::destroy()
{
    return shared_memory_object::remove(BALANCES_SEGMENT);
}

bool Balances::transfer(Transfer &_transfer, Balance &_balance)
{
    auto item = this->balances->get<by_account_currency>().find(boost::make_tuple(_transfer.account, _transfer.currency));
    if (item == this->balances->get<by_account_currency>().end())
    {
        if (_transfer.operation == Operation::Substract)
            return false;

        Balance balance(_transfer.account, _transfer.currency, _transfer.amount);
        auto result = this->balances->insert(balance);
        if (!result.second)
            return false;

        _balance = balance;
    }
    else
    {
        Balance balance(_transfer.account, _transfer.currency, 0);
        if (_transfer.operation == Operation::Add)
        {
            balance.amount = item->amount + _transfer.amount;
        }
        else
        {
            if (item->amount < _transfer.amount)
                return false;

            balance.amount = item->amount - _transfer.amount;
        }

        if (!this->balances->modify(item, [&](Balance &_b)
                                    { _b.amount = item->amount; }))
            return false;

        _balance = balance;
    }
    return true;
}

AMOUNT Balances::balance(ID _account, CURRENCY _currency)
{
    auto item = this->balances->get<by_account_currency>().find(boost::make_tuple(_account, _currency));

    return item == this->balances->get<by_account_currency>().end() ? 0 : item->amount;
}
