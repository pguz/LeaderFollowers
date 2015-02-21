#ifndef EVENTHANDLERBOOST_HPP_
#define EVENTHANDLERBOOST_HPP_

#include <zmq.hpp>
#include "mt4cpp/EventHandler.hpp"
#include "mt4cpp/Handle.hpp"

using namespace mt4cpp;

class EventHandlerBoost : public EventHandler {
public:
	EventHandlerBoost();
    virtual ~EventHandlerBoost();

private:
	virtual int handleEventImpl(PHandle handle) override;
	int processRequest(
		const std::string &requestData, 
		std::string &responseData);
		
    int counter;
};

#endif /* EVENTHANDLERBOOST_HPP_ */
