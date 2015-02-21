#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <iostream>
#include "example/BoostClient/BoostClient.hpp"

using namespace std;
using namespace boost;
using boost::unit_test::test_suite;

BOOST_AUTO_TEST_SUITE( server_tests )

BOOST_AUTO_TEST_CASE( boost_tests )
{
	BoostClient client("1", "127.0.0.1", "5557");
	std::string request_to_send = "ECHO";
	std::string response_received;
	size_t res_length = client.communicateWithServer(request_to_send, response_received);
	BOOST_CHECK(res_length > 0);
	BOOST_CHECK(request_to_send == response_received.substr(0, 4));
}

BOOST_AUTO_TEST_SUITE_END()
