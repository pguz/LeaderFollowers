#include "BoostClient.hpp"
#include <boost/range/irange.hpp>

BoostClient::BoostClient(
		std::string client_id, 
		std::string server_ip, 
		std::string port) : 
	socket_(io_service_, udp::endpoint(udp::v4(), 0)),
	resolver_(io_service_) {
	
	endpoint_ = *resolver_.resolve({udp::v4(), server_ip, port});
	templateRequestData_ = "[BOOST " + client_id + "] ";
}

void BoostClient::defaultCommunicationWithServer(
		int num_requests) {
			
	for(auto i : boost::irange(1, num_requests + 1)) {
		
		std::string request_to_send_str = std::string(templateRequestData_) + std::to_string(i);
		std::string response_received_str;
		
		std::cout << "REQ nr " << i << ": \t" << request_to_send_str << std::endl;
		
		communicateWithServer(request_to_send_str, response_received_str);
		
		std::cout << "RES nr " << i << ": \t";
		std::cout << response_received_str << std::endl;
	}
}

size_t BoostClient::communicateWithServer(
		const std::string &request_to_send_str, 
		std::string &response_received_str) {

	size_t request_length = createRequest(request_to_send_str);
	
	socket_.send_to(boost::asio::buffer(request_to_send_, request_length), endpoint_);
	
	udp::endpoint sender_endpoint;
	
	size_t response_length = socket_.receive_from(
		boost::asio::buffer(response_received_), sender_endpoint);
		
	response_received_str = std::string(response_received_.data());
		
	return response_length;
}

size_t BoostClient::createRequest(
		std::string request_to_send_str) {
	
	request_to_send_str.copy(request_to_send_, request_to_send_str.length() + 1); 
	request_to_send_[request_to_send_str.copy(request_to_send_, sizeof(request_to_send_)-1)] = '\0';
	
	return std::strlen(request_to_send_) + 1;
}
