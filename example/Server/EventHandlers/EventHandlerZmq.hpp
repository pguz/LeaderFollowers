#ifndef EVENTHANDLERZMQ_HPP_
#define EVENTHANDLERZMQ_HPP_

#include <zmq.hpp>
#include "mt4cpp/EventHandler.hpp"
#include "mt4cpp/Handle.hpp"

using namespace mt4cpp;

class EventHandlerZmq : public EventHandler {
public:
	EventHandlerZmq();
    virtual ~EventHandlerZmq();

private:
	virtual int handleEventImpl(PHandle handle) override;
	int processRequest(
		const std::string &requestData, 
		std::string &responseData);

    int counter;
};

#endif /* EVENTHANDLERZMQ_HPP_ */
