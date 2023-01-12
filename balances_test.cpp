#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include "main.h"
#include "balances.h"

#include <boost/test/unit_test.hpp>

using namespace dorrego;

struct BalanceTest
{
    BalanceTest()
    {
        Balances::destroy();
    }
    virtual ~BalanceTest()
    {
        Balances::destroy();
    }
};

BOOST_FIXTURE_TEST_SUITE(BalanceTestSuite, BalanceTest)

BOOST_AUTO_TEST_CASE(add_balance)
{
    Balances balances;
    {
        Transfer transfer(1, 1, 100, Operation::Add);
        Balance balance;
        BOOST_TEST(balances.transfer(transfer, balance));
        BOOST_TEST(balance.amount == 100);
    }
    {
        Transfer transfer(1, 1, 50, Operation::Add);
        Balance balance;
        BOOST_TEST(balances.transfer(transfer, balance));
        BOOST_TEST(balance.amount == 150);
    }
}

BOOST_AUTO_TEST_CASE(substract_balance)
{
    Balances balances;
    {
        Transfer transfer(2, 1, 100, Operation::Add);
        Balance balance;
        BOOST_TEST(balances.transfer(transfer, balance));
        BOOST_TEST(balance.amount == 100);
    }
    {
        Transfer transfer(2, 1, 70, Operation::Substract);
        Balance balance;
        BOOST_TEST(balances.transfer(transfer, balance));
        BOOST_TEST(balance.amount == 30);
    }
}

BOOST_AUTO_TEST_CASE(not_substract_balance)
{
    Balances balances;
    {
        Transfer transfer(3, 1, 10, Operation::Add);
        Balance balance;
        BOOST_TEST(balances.transfer(transfer, balance));
        BOOST_TEST(balance.amount == 10);
    }
    {
        Transfer transfer(3, 1, 50, Operation::Substract);
        Balance balance;
        BOOST_TEST(!balances.transfer(transfer, balance));
    }
}

BOOST_AUTO_TEST_CASE(balance)
{
    Balances balances;

    BOOST_TEST(balances.balance(4, 1) == 0);

    Transfer transfer(4, 1, 50, Operation::Add);
    Balance balance;
    BOOST_TEST(balances.transfer(transfer, balance));

    BOOST_TEST(balances.balance(4, 1) == 50);
    BOOST_TEST(balances.balance(5, 1) == 0);
    BOOST_TEST(balances.balance(4, 0) == 0);
}

BOOST_AUTO_TEST_SUITE_END()