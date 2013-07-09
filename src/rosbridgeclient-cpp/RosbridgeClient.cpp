/*
 * RosbridgeClient.cpp
 *
 *  Created on: 9 Jul 2013
 *      Author: marc
 */

#include "RosbridgeClient.h"

namespace rosbridge {

RosbridgeClient::~RosbridgeClient() {
	// TODO Auto-generated destructor stub
}

void RosbridgeClient::dispatch(std::string s) {
	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(s.c_str(), root);

	Json::Value msg = root["msg"];
	std::string receiver = root["receiver"].asString();
	log("receiver: " + receiver);
	log("extracted msg " + msg.toStyledString());

//	if (receiver.compare("/scan") == 0) {
//		printf("scan: ");
//		Json::Value ranges = msg["ranges"];
//		for (int i = 0; i < ranges.size(); i++) {
//			printf("%f ", ranges[i].asDouble());
//		}
//		printf("\n");
//
//	}

}

RosbridgeClient::RosbridgeClient(std::string host, std::string port) {
	this->host = host;
	this->port = port;
}

void RosbridgeClient::connect() {
	struct addrinfo *address;
	getaddrinfo(host.c_str(), port.c_str(), NULL, &address);

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	::connect(sockfd, address->ai_addr, address->ai_addrlen);
	std::string message = "raw\r\n\r\n";
	::send(sockfd, message.c_str(), message.length(), 0);
}

void RosbridgeClient::receiverThread() {
	char c;
	std::stringstream ss;
	while (true) {
		int n = read(sockfd, &c, 1);
		if (n > 0) {
			//      printf("%c [%d]",c,ss.str().length());
			if (c == 0) {
				ss.str("");
			} else if (c == -1) {
				if (ss.str().length() > 0)
					dispatch(ss.str());

				ss.str("");
			} else {
				ss << c;
			}
		}

	}

}

void RosbridgeClient::subscribe(const std::string& topic, int delay) {
	std::string cmd;
	std::ostringstream convert;
	convert << delay;
	cmd = "{\"receiver\": \"/rosbridge/subscribe\", \"msg\": [\"" + topic
			+ "\", " + convert.str() + "]}";
	send(cmd);
}

void RosbridgeClient::announce(const std::string& topic) {
}

void RosbridgeClient::publish(const std::string& topic, const std::string& type,
		const Json::Value& msg) {
	std::string cmd;
	std::ostringstream convert;
	cmd = "{\"type\": \"" + type + "\", \"receiver\": \"" + topic
			+ "\", \"msg\": " + msg.toStyledString() + "}";
	send(cmd);
}

void RosbridgeClient::wait() {
}

ValuePtr RosbridgeClient::readLastValue(const std::string& topic) {
	return ValuePtr(new Json::Value());
}

void RosbridgeClient::spin() {
	boost::thread workerThread(
			boost::bind(&RosbridgeClient::receiverThread, this));
}

void RosbridgeClient::run() {
	spin();
	while (true) {
		sleep(10);
	}
}

void RosbridgeClient::log(std::string s) {
	std::cerr << "[RosbridgeClient] " << s << std::endl;
}

void RosbridgeClient::publish(const std::string& topic,
		const std::string& msg) {
	Json::Value root;
	root["data"] = Json::Value(msg);

	publish(topic, "std_msgs/String", root);
}

void RosbridgeClient::send(std::string s) {
	std::string m = " " + s + "\xff";
	//std::string m =  s + "\xff";
	log("send: " + m);
	::send(sockfd, m.c_str(), strlen(m.c_str()), 0);

}

}
/* namespace rosbridge */

int main(int argc, char** argv) {
	rosbridge::RosbridgeClientPtr rbc = rosbridge::RosbridgeClientPtr(
			new rosbridge::RosbridgeClient("localhost", "9090"));
	rbc->connect();
	rbc->subscribe("/odom", 1000);

	Json::Value root;
	root["data"] = Json::Value("Test");
	rbc->publish("/hurga", "test2");
	sleep(1);
	rbc->publish("/hurga", "std_msgs/String", root);
	rbc->run();


//  struct addrinfo *address;
//  getaddrinfo("marcs-pc", "9090", NULL, &address);
//
//  sockfd = socket(PF_INET, SOCK_STREAM, 0);
//  connect(sockfd, address->ai_addr, address->ai_addrlen);
//
//  char* message = "raw\r\n\r\n";
//  send(sockfd, message, strlen(message),0);
//
//  std::string m = ("\x00"+std::string(argv[1])+"\xff");
//  send(sockfd, m.c_str(), strlen(m.c_str()),0);
//
//  boost::thread workerThread(receiverThread);
//
//  sleep(10);
//  workerThread.join();
//

}

