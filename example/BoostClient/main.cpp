#include "BoostClient.hpp"
#include <boost/lexical_cast.hpp>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 5) {
		std::cerr << "Usage: blocking_udp_echo_client client_id[int] host[string] port[string] num_requests[int]\n";
		return 1;
    }
    
    int num_requests;
    try {
		num_requests = boost::lexical_cast<int>(argv[4]);
	} catch(const boost::bad_lexical_cast& e) {
		std::cerr << "Not proper format argument" << std::endl;
		std::cerr << "Caught bad lexical cast with error " << e.what() << std::endl;
		return 1;
	}
	
	std::string client_id = argv[1];
	std::string server_ip = argv[2];
	std::string port = argv[3];
	 
    try {
		BoostClient client(client_id, server_ip, port);
		client.defaultCommunicationWithServer(num_requests);
    } catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	
	
	
	return 0;
}
