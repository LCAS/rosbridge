/*
 * main.cpp
 *
 *  Created on: 9 Jul 2013
 *      Author: marc
 */

#include "RosbridgeClient.h"

int main(int argc, char** argv) {
	rosbridge::RosbridgeClientPtr rbc = rosbridge::RosbridgeClientPtr(
			new rosbridge::RosbridgeClient("localhost", "9090"));
	rbc->connect();
	rbc->subscribe("/odom", 1000);
	rbc->subscribe("/scan", 1000);
	rbc->subscribe("/amcl_pose", 1000);

	Json::Value root;
	root["data"] = Json::Value("Test");
	rbc->publish("/hurga", "test2");

	rbc->publish("/hurga", "std_msgs/String", root);
	rbc->spin();
	while(true) {
		rbc->log(rbc->readLastValue("/odom")->toStyledString());
		rbc->wait();
	}

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



