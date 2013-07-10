/*
 * SGTBridge.h
 *
 *  Created on: 9 Jul 2013
 *      Author: marc
 */

#ifndef SGTBRIDGE_H_
#define SGTBRIDGE_H_

#include "RosbridgeClient.h"
#include <vector>


namespace rosbridge {

class SGTBridge: public rosbridge::RosbridgeClient {
	static std::string templ;
	Json::Value goalTemplateValue;
public:
	typedef struct {
		std::vector<double> ranges;
		std::vector<double> angle;
		int secs;
		int nsecs;
	} Scan;

	typedef struct {
		double x;
		double y;
		double t;
		double dx;
		double dy;
		double dt;
		int secs;
		int nsecs;
	} Pose;

	SGTBridge(std::string host, std::string port);
	virtual ~SGTBridge();

	Scan getScan();
	bool goTo(Pose p);
	Pose getPose();

};

} /* namespace rosbridge */
#endif /* SGTBRIDGE_H_ */
