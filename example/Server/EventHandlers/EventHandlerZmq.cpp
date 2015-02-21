#include <iostream>
#include <zmq.hpp>
#include "EventHandlerZmq.hpp"
#include "../Handles/HandleZmq.hpp"

EventHandlerZmq::EventHandlerZmq()
    : EventHandler(),
      counter(0) {
}

EventHandlerZmq::~EventHandlerZmq() {
}

int EventHandlerZmq::handleEventImpl(
	PHandle handle) {
		
	HandleZmq* handleZmq = dynamic_cast<HandleZmq*>(handle.get());
	
	if(handleZmq == nullptr)
		return -1;
	
    ++counter;
    
	std::string responseData;
    processRequest(handleZmq->getData(), responseData);
   
	handleZmq->sendResponse(responseData);

	return 0;
}

int EventHandlerZmq::processRequest(
	const std::string &requestData, 
	std::string &responseData) {
		
	std::cout << "[EventHandlerZmq] \tREQ nr: " << counter << "\t" << requestData << std::endl;
	responseData = requestData + " (" + std::to_string (counter) + ")";
	std::cout << responseData << std::endl;
	
	return 0;
}
