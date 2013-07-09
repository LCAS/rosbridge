#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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


#define BUFSIZE 1


char buffer[BUFSIZE];
int sockfd;

Json::Value root;  
Json::Reader reader;



void dispatch(std::string s){ 
  //printf("MSG:\n%s\n\n\n",s.c_str());
  bool parsingSuccessful = reader.parse( s.c_str(), root );
  Json::Value msg = root["msg"];
  std::string receiver=root["receiver"].asString();
  printf("receiver: %s\n",receiver.c_str());
  printf("extracted msg: %s\n",msg.toStyledString().c_str());

  if (receiver.compare("/scan")==0) {
    printf("scan: ");
    Json::Value ranges=msg["ranges"]; 
    for (int i=0; i<ranges.size(); i++) {
      printf("%f ",ranges[i].asDouble());
    }
    printf("\n");

  }

}

void receiverThread() {
  char c;
  std::stringstream ss;
  while(true) {
    int n=read(sockfd, &c, 1);
    if (n>0) {
      //      printf("%c [%d]",c,ss.str().length());
      if (c==0) {
	ss.str("");
      }
      else if (c==-1) {
	if (ss.str().length()>0)
	  dispatch(ss.str());
	ss.str("");
      }
      else {
	ss << c;
      }
      
    }
  }
}


int main(int argc, char** argv) {
  struct addrinfo *address;
  getaddrinfo("marcs-pc", "9090", NULL, &address);
  
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  connect(sockfd, address->ai_addr, address->ai_addrlen);
  
  char* message = "raw\r\n\r\n";
  send(sockfd, message, strlen(message),0);
  
  std::string m = ("\x00"+std::string(argv[1])+"\xff");
  send(sockfd, m.c_str(), strlen(m.c_str()),0);

  boost::thread workerThread(receiverThread);
  
  sleep(10);
  workerThread.join();  
  
  
}
