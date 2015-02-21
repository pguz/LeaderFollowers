#ifndef REACTOR_HPP_
#define REACTOR_HPP_

#include <vector>
#include <boost/thread.hpp>
#include "Handle.hpp"

namespace mt4cpp {
	
	typedef std::vector<PHandle> PHandleVec;

	//Encapsulating the lower-level handle set mechanisms
	//with a higher-level Reactor
	class Reactor {
	private:
		Reactor() {}
		Reactor(const Reactor&) = delete;
		Reactor& operator=(const Reactor&) = delete;
		
	public:
	   
		static Reactor& getInstance(){
			static Reactor instance;
			return instance;
		}
		
		void setTimeoutForHandles(const int timeout) {
			defTimeout_ = timeout;
		}

		//Register a handle
		void registerHandle(
				PHandle handle) {
			handles_.push_back(handle);
		}

		void handleEvent(
				PHandle handle) {
			handle->handleMe();
		}

		//Leader demultiplexing:
		// The leader waits for an event
		// to occur on the handle set
		PHandle runEventLoop(volatile bool &shouldStop) {
			while (true) {
				//Iterative handle set
				for (auto iHandle = handles_.begin();
						iHandle != handles_.end(); ++iHandle) {
							
	//				try {
	//					boost::this_thread::interruption_point();
	//				} catch (boost::thread_interrupted&) {
	//					return nullptr;
	//				}

					if(shouldStop)
						return nullptr;

					if(!((*iHandle)->isActivated()))
						continue;
					int res = (*iHandle)->handle(defTimeout_);
					if(res == -1)
						continue;
					(*iHandle)->setActivated(false);
					
					return (*iHandle);
				}
			}
		}

	protected:
		PHandleVec handles_;
		int defTimeout_;
	};
	
} //namespace mt4cpp

#endif /* REACTOR_HPP_ */
