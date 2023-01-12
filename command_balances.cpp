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

using namespace dorrego;
using namespace boost::interprocess;

template <>
void Command<Balances, Transfer, Balance>::execute()
{
    try
    {
        scoped_lock<named_mutex> lock(this->mutex);

        this->book.transfer(this->request, this->response);
    }
    catch (interprocess_exception &_ex)
    {
        std::cerr << _ex.what() << std::endl
                  << std::flush;
    }
}