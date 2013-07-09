/*
 * RosbridgeClient.h
 *
 *  Created on: 9 Jul 2013
 *      Author: marc
 */

#ifndef ROSBRIDGECLIENT_H_
#define ROSBRIDGECLIENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <sstream>
#include "json/json.h"
#include <boost/smart_ptr.hpp>

namespace rosbridge {

class RosbridgeClient;

typedef boost::shared_ptr<Json::Value> ValuePtr;
typedef boost::shared_ptr<rosbridge::RosbridgeClient> RosbridgeClientPtr;

class RosbridgeClient {
	std::string host;
	std::string port;
	int sockfd;

public:
	RosbridgeClient(std::string host, std::string port);
	void connect();
	virtual ~RosbridgeClient();
	void subscribe(const std::string& topic, int delay = -1);
	void announce(const std::string& topic);
	void publish(const std::string& topic, const std::string& type,
			const Json::Value& msg);
	void publish(const std::string& topic, const std::string& msg);
	void wait();
	ValuePtr readLastValue(const std::string& topic);
	void spin();
	void run();
	void log(std::string s);
protected:
	void dispatch(std::string s);
	void receiverThread();
	void send(std::string s);
};

} /* namespace rosbridge */
#endif /* ROSBRIDGECLIENT_H_ */
