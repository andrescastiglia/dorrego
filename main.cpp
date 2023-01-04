#include <boost/asio.hpp>
#include "saver.h"
#include "listener.h"
#include "in_channel.h"

#include <boost/mpi.hpp>

#include <iostream>

namespace mpi = boost::mpi;

int main(int argc, char* argv[])
{
    mpi::environment env(argc, argv);
    mpi::communicator world;
    std::cout << "I am process " << world.rank() << " of " << world.size() << "." << std::endl;

    InChannel inChannel;

    return EXIT_SUCCESS;
}