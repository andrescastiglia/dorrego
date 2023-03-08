#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>

#include "main.h"
#include "balances.h"
#include "command.h"

#define MUTEX_NAME "DBM%ld.%d"

using namespace dorrego;
using namespace boost::interprocess;

template <>
Command<Balances, Transfer, Balance>::Command(Balances &_book, Transfer &_request, Balance &_response)
    : book(_book), request(_request), response(_response), succeeded(false)
{
    int len = snprintf(nullptr, 0, MUTEX_NAME, this->request.account, this->request.currency);

    this->mutex_name = (char *)malloc((len + 1) * sizeof(char));

    snprintf(this->mutex_name, len, MUTEX_NAME, this->request.account, this->request.currency);
}

template <>
Command<Balances, Transfer, Balance>::~Command()
{
    free(this->mutex_name);
}

template <>
void Command<Balances, Transfer, Balance>::execute()
{
    try
    {
        named_mutex mutex(open_or_create, this->mutex_name);
        {
            scoped_lock<named_mutex> lock(mutex);

            this->succeeded = this->book.transfer(this->request, this->response);
        }
    }
    catch (interprocess_exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}

template <>
void Command<Balances, Transfer, Balance>::rollback()
{
    if (!this->succeeded)
        return;

    Transfer reverse = this->request;
    reverse.operation = this->request.operation == Operation::Add ? Operation::Substract : Operation::Add;

    try
    {
        named_mutex mutex(open_or_create, this->mutex_name);
        {
            scoped_lock<named_mutex> lock(mutex);

            this->succeeded = this->book.transfer(reverse, this->response);
        }
    }
    catch (interprocess_exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}