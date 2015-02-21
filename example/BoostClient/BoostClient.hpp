#ifndef BOOSTCLIENT_HPP_
#define BOOSTCLIENT_HPP_

#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

class BoostClient {
public:
	
	BoostClient(
		std::string client_id, 
		std::string server_ip, 
		std::string port);
	
	void defaultCommunicationWithServer(
		int num_requests);
		
	size_t communicateWithServer(
		const std::string &request_to_send_str, 
		std::string &response_received_str);
	
private:
    boost::asio::io_service io_service_;
    udp::socket socket_;
    udp::resolver resolver_;
    udp::endpoint endpoint_;
    
    char request_to_send_[1024];
    boost::array<char, 1024> response_received_;
    std::string templateRequestData_;
    
    size_t createRequest(
		std::string request_to_send_str_);
};

#endif /* BOOSTCLIENT_HPP_ */
