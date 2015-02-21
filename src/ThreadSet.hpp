#ifndef THREADSET_HPP_
#define THREADSET_HPP_

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/tss.hpp>
#include "Reactor.hpp"
#include "Handle.hpp"

namespace mt4cpp {
	
	namespace {
		struct ThreadInfo{
			ThreadInfo() 
				: numLeader(0), 
				  numFollower(0), 
				  numProcessing(0) 
				  {}

			int numLeader;
			int numFollower;
			int numProcessing;
		};
		
		boost::thread_specific_ptr<ThreadInfo> threadInfo;
	}  

	class ThreadSet {
	public:
		ThreadSet()  :
			leaderId_(boost::thread::id()),
			stopped_(false),
			numLeaders_(0),
			numFollowers_(0),
			numFollowersCondition_(0),
			numProcessing_(0) {}

		virtual ~ThreadSet() {}

		void stop() {
			stopped_ = true;
		}

		void run() {
			//init ThreadInfo - thread specific storage for every thread
			threadInfo.reset(new ThreadInfo());
			join();
		}

		void join() {
			//State Follower: begins
			incFollowers();
			mtxJoin_.lock();
			
			//Interruption point
			if(!runFollower())
				return;
			
			while(true) {
				
				//for diagnosis only, there should be one thread at a time
				incFollowersCondition();
				
				boost::mutex::scoped_lock lock(mtxFollowersCondition_);
				
				//Interruption point
				if(!runFollowerCondition())
					return;
				
				//Dynamics 4.: Rejoining the leader/followers thread set
				while(leaderId_ != boost::thread::id()) {
					//Waiting to become the leader
					followersCondition_.wait(lock); //preleader
					
					//Interruption point
					if(!runFollowerCondition())
						return;
				}
				
				//for diagnosis only, there should be one thread at a time
				decFollowersCondition();
				
				//State Follower: ends
				//State Leader: begins
				decFollowers();
				leaderId_ = boost::this_thread::get_id();
				incLeaders();

				lock.unlock();
				
				//leave monitor temporarily to allow other follower threads to join the set
				mtxJoin_.unlock();

				//Dynamics 1.: Leader demultiplexing, 
				PHandle handle = Reactor::getInstance().runEventLoop(stopped_);

				//Interruption point from runEventLoop
				if(stopped_ == true) {
					freeLeader();
					return;
				}

				//Dynamics 2.: Follower promotion
				promoteNewLeader();

				//State Leader: ends
				//State Processing: begins
				incProcessing();

				//Dynamics 3.: Event processing
				Reactor::getInstance().handleEvent(handle);
				handle->setActivated(true);

				//State Processing: ends
				//State Follower: begins
				decProcessing();
				incFollowers();
				
				mtxJoin_.lock();
				
				//Interruption point
				if(!runFollower())
					return;
			}
		}

		void promoteNewLeader() {
			//promoting a follower thread to become
			//the new leader
			mtxFollowersCondition_.lock();
	
			//lider loses his position
			leaderId_ = boost::thread::id();
			decLeaders();
	
			followersCondition_.notify_one();
			mtxFollowersCondition_.unlock();
			
			return;
		}

	private:

		// The thread id of the leader thread, which is
		// set to NO_CURRENT_LEADER if there is no leader.
		boost::thread::id leaderId_;

		// Follower threads wait on this condition
		// variable until they are promoted to leader.
		boost::condition_variable followersCondition_;

		// Serialize access to our internal state.
		boost::mutex mtxFollowersCondition_;
		boost::mutex mtxJoin_;

		volatile bool stopped_;
		
		void freeLeader() {
			decLeaders();
			
			dispThreadInfo();
			
			if(numFollowersCondition_ > 0) {
				mtxFollowersCondition_.lock();
				followersCondition_.notify_one();
				mtxFollowersCondition_.unlock();
			}
			
			if(numFollowers_ > 0)
				mtxJoin_.unlock(); 
		}
		
		void freeFollowers() {
			decFollowers();
			
			dispThreadInfo();
			if(numFollowers_ > 0)
				mtxJoin_.unlock();
		}
		
		bool runFollower() {
			if(stopped_) {
				freeFollowers();
				return false;
			}
			
			return true;
		}
		
		bool runFollowerCondition() {
			if(stopped_) {
				decFollowersCondition();
				dispThreadInfo();
				return false;
			}
			return true;
		}
		
		/* Diagnosis begin*/
		
		int numLeaders_;
		int numFollowers_;
		int numFollowersCondition_;
		int numProcessing_;
		
		boost::mutex mtxNumLeaders_;
		boost::mutex mtxNumFollowers_;
		boost::mutex mtxNumFollowersCondition_;
		boost::mutex mtxNumProcessing_;

		void incLeaders() {
			boost::mutex::scoped_lock lock(mtxNumLeaders_);
			
			ThreadInfo* info = threadInfo.get();
			++(info->numLeader);
			
			++numLeaders_;
			BOOST_ASSERT_MSG(numLeaders_ == 1, "There should be one leader!");
		}
		
		void decLeaders() {
			boost::mutex::scoped_lock lock(mtxNumLeaders_);
			--numLeaders_;
			BOOST_ASSERT_MSG(numLeaders_ == 0, "There should be zero leader!");	
		}
		
		void incFollowers() {
			boost::mutex::scoped_lock lock(mtxNumFollowers_);
			
			ThreadInfo* info = threadInfo.get();
			++(info->numFollower);
			
			++numFollowers_;
		}
		
		void decFollowers() {
			boost::mutex::scoped_lock lock(mtxNumFollowers_);
			--numFollowers_;
		}
		
		void incFollowersCondition() {
			boost::mutex::scoped_lock lock(mtxNumFollowersCondition_);
			++numFollowersCondition_;
			BOOST_ASSERT_MSG(numFollowersCondition_ == 1, "There should be one preLeader!");
		}
		
		void decFollowersCondition() {
			boost::mutex::scoped_lock lock(mtxNumFollowersCondition_);
			--numFollowersCondition_;
			BOOST_ASSERT_MSG(numFollowersCondition_ == 0, "There should be zero preLeaders!");
		}

		void incProcessing() {
			boost::mutex::scoped_lock lock(mtxNumProcessing_);
			
			ThreadInfo* info = threadInfo.get();
			++(info->numProcessing);
			
			++numProcessing_;
		}
		
		void decProcessing() {
			boost::mutex::scoped_lock lock(mtxNumProcessing_);
			--numProcessing_;
		}
		
		void dispThreadInfo() {
			ThreadInfo* info = threadInfo.get();
			std::cout << "\n" << std::endl;
			std::cout << "Thread Id:\t" << boost::this_thread::get_id() << std::endl;
			std::cout << "numLeader:\t" << info->numLeader << std::endl;
			std::cout << "numFollower:\t" << info->numFollower << std::endl;
			std::cout << "numProcessing:\t" << info->numProcessing << std::endl;
			std::cout << "\n" << std::endl;
		}
		
		/* Diagnosis end*/
	};

} //namespace mt4cpp

#endif /* THREADSET_HPP_ */
