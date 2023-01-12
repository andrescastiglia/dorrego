#pragma once

#define TRANSFER_SIZE sizeof(struct Transfer)
#define BALANCE_SIZE sizeof(struct Balance)
#define BALANCES_SIZE (BALANCE_SIZE * 1024)

namespace dorrego
{
    using namespace boost::interprocess;
    using namespace boost::multi_index;

    enum Operation
    {
        Add,
        Substract
    };

    struct Transfer : Request
    {
        Transfer() = default;
        Transfer(ID _account, CURRENCY _currency, AMOUNT _amount, Operation _operation) : account(_account), currency(_currency), amount(_amount), operation(_operation)
        {
        }
        ID account;
        CURRENCY currency;
        AMOUNT amount;
        Operation operation;
    };

    struct Balance : Response
    {
        Balance() = default;
        Balance(ID _account, CURRENCY _currency, AMOUNT _amount) : account(_account), currency(_currency), amount(_amount)
        {
        }
        ID account;
        CURRENCY currency;
        AMOUNT amount;
    };

    typedef boost::multi_index_container<
        Balance,
        indexed_by<
            ordered_unique<tag<struct by_account_currency>,
                           composite_key<Balance,
                                         member<Balance, ID, &Balance::account>,
                                         member<Balance, CURRENCY, &Balance::currency>>>>,
        allocator<Balance, managed_shared_memory::segment_manager>>
        BalancesContainer;

    class Balances : public Book
    {
    private:
        managed_shared_memory segment;
        BalancesContainer *balances;

    public:
        Balances();
        virtual ~Balances() = default;

        static bool destroy();

        bool transfer(Transfer &_transfer, Balance &_balance);
        AMOUNT balance(ID _account, CURRENCY _currency);
    };
}