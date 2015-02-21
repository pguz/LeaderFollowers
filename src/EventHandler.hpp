#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

#include <memory>
/*
 * The set of operations available to process events
 * that occur on handles
 */
 
namespace mt4cpp {
	
	class Handle; 
	typedef std::shared_ptr<Handle> PHandle;
		
	class EventHandler {
	public:
		EventHandler() {}
		virtual ~EventHandler() {}

		int handleEvent(
			PHandle handle) {
		
			return handleEventImpl(handle);
		}
		
		void update(
			PHandle handle) {
				
			handleEvent(handle);
		}

	private:

		//hook method dispatched by a Reactor
		//to handle events
		virtual int handleEventImpl(
			PHandle handle) = 0;

	};

	typedef std::shared_ptr<EventHandler> PEventHandler;

} //namespace mt4cpp

#endif /* EVENTHANDLER_HPP_ */
