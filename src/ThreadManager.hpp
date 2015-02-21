#ifndef THREADMANAGER_HPP_
#define THREADMANAGER_HPP_

#include "ThreadSet.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

namespace mt4cpp {
	
	namespace {
		void threadFunction(void *p_thread_set) {
			ThreadSet *thread_set = static_cast<ThreadSet*>(p_thread_set);
			thread_set->run();
		}
	}

	class ThreadManager {
	public:
		ThreadManager(int numberOfThreads) :
			numberOfThreads_(numberOfThreads),
			threadSet_(new ThreadSet()),
			signals_(ioService_), 
			work_(ioService_) {
				
			signals_.add(SIGINT);
			signals_.add(SIGTERM);
				
		}

		virtual ~ThreadManager() {
			delete threadSet_;
		}

		void run() {
			
			for(int i = 0; i < numberOfThreads_; ++i) {
				threadpool_.create_thread(
					boost::bind(&boost::asio::io_service::run, &ioService_)
				);
			}

			signals_.async_wait(boost::bind(&ThreadManager::signalHandler, this));

			//Unbound association
			for(int i = 0; i < numberOfThreads_; ++i) {
				ioService_.post(boost::bind(&threadFunction, threadSet_));
			}

			ioService_.run();
			threadpool_.join_all();
		}

	private:
		void signalHandler() {
			
			threadSet_->stop();
			ioService_.stop();
			signals_.async_wait(boost::bind(&ThreadManager::signalHandler, this));
		}

		int numberOfThreads_;
		ThreadSet *threadSet_;
		boost::asio::io_service ioService_;
		boost::asio::signal_set signals_;
		boost::thread_group threadpool_;
		boost::asio::io_service::work work_;

	};
	
} //namespace mt4cpp

#endif /* THREADMANAGER_HPP_ */
