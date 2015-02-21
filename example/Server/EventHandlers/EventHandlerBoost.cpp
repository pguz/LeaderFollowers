#include <iostream>
#include "../EventHandlers/EventHandlerBoost.hpp"
#include "../Handles/HandleBoost.hpp"

EventHandlerBoost::EventHandlerBoost()
    : EventHandler(),
      counter(0) {
}

EventHandlerBoost::~EventHandlerBoost() {
}

int EventHandlerBoost::handleEventImpl(
	PHandle handle) {

	HandleBoost* handleBoost = dynamic_cast<HandleBoost*>(handle.get());
	
	if(handleBoost == nullptr)
		return -1;

    ++counter;
    
	std::string responseData;
    processRequest(handleBoost->getData(), responseData);
   
	handleBoost->sendResponse(responseData);

	return 0;
}

int EventHandlerBoost::processRequest(
	const std::string &requestData, 
	std::string &responseData) {
		
	std::cout << "[EventHandlerBoost] \tREQ nr: " << counter << "\t" << requestData << std::endl;
	responseData = requestData + " (" + std::to_string (counter) + ")";
	std::cout << responseData << std::endl;
	return 0;
}
