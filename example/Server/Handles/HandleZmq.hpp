#ifndef HANDLEZMQ_HPP_
#define HANDLEZMQ_HPP_

#include <zmq.hpp>
#include "mt4cpp/Handle.hpp"

using namespace mt4cpp;

class HandleZmq : public Handle {
public:
    HandleZmq(std::string hostAddress, int port);
    virtual ~HandleZmq();

    int handle(const int timeout) override;

	std::string getData();
	int sendResponse(std::string response);
	
private:
    zmq::context_t context_;
    zmq::socket_t socket_;
    zmq::pollitem_t items_[1];
    
    std::string data_;
};

#endif /* HANDLEZMQ_HPP_ */
