#define BOOST_TEST_MODULE DorregoTest
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

struct ConfigTest
{
    ConfigTest()
    {
        unit_test_log.set_format(OF_XML); //OF_JUNIT
    }
};

BOOST_TEST_GLOBAL_CONFIGURATION(ConfigTest);
