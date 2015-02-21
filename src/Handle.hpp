#ifndef HANDLE_HPP_
#define HANDLE_HPP_

#include <memory>
#include <vector>
#include "EventHandler.hpp"

namespace mt4cpp {
	
	typedef std::vector<PEventHandler> PEventHandlerVec;
		
	/*Handle identifies I/O resources*/
	class Handle : public std::enable_shared_from_this<mt4cpp::Handle> {
	public:
		Handle() : activated(true) {}
		virtual ~Handle() {}

		//should queue events internally
		virtual int handle(const int timeout) = 0;

		void addEventHandler(PEventHandler eventHandler) {
			obs_.push_back(eventHandler);
		}

		void handleMe() {
			notify();
		}
		
		void setActivated(bool activated) {
			this->activated = activated;
		}
		
		bool isActivated() {
			return activated;
		}
		
	private:
		bool activated;
		PEventHandlerVec obs_;

		void notify() {
			for(PEventHandler eventHandler : obs_)
				eventHandler->update(shared_from_this());
		}
	};

	typedef std::shared_ptr<Handle> PHandle;

} //namespace mt4cpp

#endif /* HANDLE_HPP_ */
