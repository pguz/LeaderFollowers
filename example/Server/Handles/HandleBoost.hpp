#ifndef HANDLEBOOST_HPP_
#define HANDLEBOOST_HPP_

#include "mt4cpp/Handle.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;
using namespace mt4cpp;

struct BoostAdapterInUnix {
	struct timeval timeStruct;
	fd_set fileDescriptorSet;
	int nativeSocket;

	bool timeout(udp::socket &sock, int timeout) {
		timeStruct.tv_sec = 0;
		timeStruct.tv_usec = timeout * 1000;
		
		FD_ZERO(&fileDescriptorSet);

		nativeSocket = sock.native();
		FD_SET(nativeSocket, &fileDescriptorSet);

		select(nativeSocket + 1, &fileDescriptorSet, NULL, NULL, &timeStruct);
		if(!FD_ISSET(nativeSocket, &fileDescriptorSet)) {
			return true;
		}
		return false;
	}	

};

class HandleBoost : public Handle {
public:
    HandleBoost(unsigned int port);
    virtual ~HandleBoost();

    int handle(const int timeout) override;
    
    std::string getData();
	int sendResponse(std::string response);

private:
	boost::asio::io_service io_service_;
	unsigned short port_;
	udp::socket socket_;
	udp::endpoint sender_endpoint_;
	BoostAdapterInUnix nativeSocket_;
	
	std::string data_;
  };

#endif /* HANDLEBOOST_H_ */
