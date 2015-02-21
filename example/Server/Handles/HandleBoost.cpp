#include "HandleBoost.hpp"
#include <iostream>
#include <unistd.h>

enum { max_length = 1024 };

HandleBoost::HandleBoost(
		unsigned int port)
    : Handle(),
      port_(port),
      socket_(io_service_, udp::endpoint(udp::v4(), port)) {	
}

HandleBoost::~HandleBoost() {
	socket_.close();
}

int HandleBoost::handle(const int timeout) {
	char requestData[max_length];

	if(nativeSocket_.timeout(socket_, timeout))
		return -1;
		
	usleep(10000);
		
	/*size_t length = */socket_.receive_from(boost::asio::buffer(requestData, max_length), sender_endpoint_);

	data_ = std::string(requestData); 
	
	return 0;
}

std::string HandleBoost::getData() {
    return data_;
}

int HandleBoost::sendResponse(std::string response) {
	socket_.send_to(boost::asio::buffer(response, response.size()), sender_endpoint_);
	return 0;
}


