#include <iostream>
#include "mt4cpp/Reactor.hpp"
#include "mt4cpp/ThreadManager.hpp"
#include "example/Server/Handles/HandleZmq.hpp"
#include "example/Server/EventHandlers/EventHandlerZmq.hpp"

#ifdef __unix__  
	#include "example/Server/Handles/HandleBoost.hpp"
	#include "example/Server/EventHandlers/EventHandlerBoost.hpp"
#endif

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace mt4cpp;

void printUsage(std::string program_name) {
	std::cout << "Usage: ./" << program_name
			<< " config_file[string]" << std::endl;
}

bool readConfig(
	const std::string &config_file,
	std::string &host_address,
	int &num_of_threads,
	int &timeout,
	int &zmq_port,
	int &boost_port) {
	
	try {
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("config.ini", pt);
		host_address = pt.get<std::string>("MAIN.host_address");
		num_of_threads = pt.get<int>("MAIN.num_of_threads");
		timeout = pt.get<int>("MAIN.timeout");
		zmq_port = pt.get<int>("ZMQ.port");
		boost_port = pt.get<int>("BOOST.port");
	} catch(std::exception const&  ex) {
		std::cerr << ex.what() << std::endl;
		return false;
	}
	
	return true;
}

int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		printUsage(argv[0]);
		return 1;
	}
	
	std::string host_address;
    int num_of_threads;
    int timeout;
    int zmq_port;
	int boost_port;
	
	readConfig(argv[1], host_address, num_of_threads, timeout, zmq_port, boost_port);
	Reactor::getInstance().setTimeoutForHandles(timeout);

    PEventHandler zmqEventHandler = PEventHandler(new EventHandlerZmq());
    PHandle handleZmq = PHandle(new HandleZmq(host_address, zmq_port));
    handleZmq->addEventHandler(zmqEventHandler);
    Reactor::getInstance().registerHandle(handleZmq);

	#ifdef __unix__    
		PEventHandler boostEventHandler = PEventHandler(new EventHandlerBoost());
		PHandle handleBoost = PHandle(new HandleBoost(boost_port));
		handleBoost->addEventHandler(boostEventHandler);
		Reactor::getInstance().registerHandle(handleBoost);
	#endif
	   
    ThreadManager threadManager(num_of_threads);
    threadManager.run();

    return 0;
}
