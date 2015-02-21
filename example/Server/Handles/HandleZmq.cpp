#include "HandleZmq.hpp"
#include <iostream>

HandleZmq::HandleZmq(std::string hostAddress, int port)
    : Handle(),
      context_(1),
      socket_(context_, ZMQ_REP)/*,
      items{{ socket, 0, ZMQ_POLLIN, 0 }}*/{
	items_[0] = { socket_, 0, ZMQ_POLLIN, 0 };	// vs2013 needs that
	std::string full_address = "tcp://" + hostAddress + ":" + std::to_string(port);
    socket_.bind(full_address.c_str());
}

HandleZmq::~HandleZmq() {
}

int HandleZmq::handle(const int timeout) {
     zmq_poll(items_, 1, timeout);
     if (items_[0].revents & ZMQ_POLLIN) {
		zmq::message_t request;
		int t = 10;
		socket_.setsockopt(ZMQ_RCVTIMEO,&t,sizeof(t));
		socket_.recv(&request);

		data_ = std::string(static_cast<char*>(request.data()), request.size());

        return 0;
     }
     return -1;
}

std::string HandleZmq::getData() {
    return data_;
}

int HandleZmq::sendResponse(std::string response) {
	zmq::message_t reply (response.size());
	memcpy ((void *) reply.data (), response.c_str(), response.size());
	socket_.send (reply);
	return 0;
}
