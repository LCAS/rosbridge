/*
 * SGTBridge.cpp
 *
 *  Created on: 9 Jul 2013
 *      Author: marc
 */

#include "SGTBridge.h"

using namespace Json;
using namespace std;

namespace rosbridge {

std::string SGTBridge::templ = "{"
		"   \"header\" : {"
		"      \"frame_id\" : \"/odom\","
		"      \"seq\" : 0,"
		"      \"stamp\" : {"
		"         \"nsecs\" : 0,"
		"         \"secs\" : 0"
		"      }"
		"   },"
		"   \"pose\" : {"
		"      \"orientation\" : {"
		"         \"w\" : 1.0,"
		"         \"x\" : 0.0,"
		"         \"y\" : 0.0,"
		"         \"z\" : 0.0"
		"         },"
		"      \"position\" : {"
		"         \"x\" : 0.0,"
		"         \"y\" : 0.0,"
		"         \"z\" : 0.0"
		"      }"
		"   }"
		"}";

SGTBridge::SGTBridge(std::string host, std::string port) :
		RosbridgeClient(host, port) {

	Reader reader;
	reader.parse(templ, goalTemplateValue);
	printf("%s\n", goalTemplateValue.toStyledString().c_str());

}

SGTBridge::~SGTBridge() {
	// TODO Auto-generated destructor stub
}

SGTBridge::Scan SGTBridge::getScan() {
	ValuePtr scanMsg = readLastValue("/scan");
	SGTBridge::Scan res;
	if (!scanMsg->isNull()) {
		res.secs = (((*scanMsg)["header"])["stamp"])["secs"].asInt();
		res.nsecs = (((*scanMsg)["header"])["stamp"])["nsecs"].asInt();
		Value r = (*scanMsg)["ranges"];
		double angle_min = (*scanMsg)["angle_min"].asDouble();
		double angle_max = (*scanMsg)["angle_max"].asDouble();
		double angle_increment = (*scanMsg)["angle_increment"].asDouble();
		double angle=angle_min;
		for (int i = 0; i < r.size(); i++) {
			res.ranges.push_back(r[i].asDouble());
			res.angle.push_back(angle);
			angle+=angle_increment;
		}
	}
	return res;
}

bool SGTBridge::goTo(SGTBridge::Pose p) {
	Value goalMsg(goalTemplateValue);
	struct timeval tv;
	gettimeofday(&tv, NULL);
	goalMsg["header"]["stamp"]["secs"]=(int) tv.tv_sec;
	goalMsg["header"]["stamp"]["nsecs"]=(int) tv.tv_usec*1000;
	goalMsg["pose"]["position"]["x"] = p.x;
	goalMsg["pose"]["position"]["y"] = p.y;
	goalMsg["pose"]["orientation"]["z"] = sin(p.t / 2.0);
	goalMsg["pose"]["orientation"]["w"] = cos(p.t / 2.0);
	log("send goal: " + goalMsg.toStyledString());
	publish("/move_base/simple_goal","geometry_msgs/PoseStamped", goalMsg);
	return 1;
}

SGTBridge::Pose SGTBridge::getPose() {
	SGTBridge::Pose res;
	ValuePtr scanMsg = readLastValue("/odom");
	if (!scanMsg->isNull()) {
		res.secs = (((*scanMsg)["header"])["stamp"])["secs"].asInt();
		res.nsecs = (((*scanMsg)["header"])["stamp"])["nsecs"].asInt();
		res.x = (*scanMsg)["pose"]["pose"]["position"]["x"].asDouble();
		res.y = (*scanMsg)["pose"]["pose"]["position"]["y"].asDouble();
		double z = (*scanMsg)["pose"]["pose"]["orientation"]["z"].asDouble();
		res.t = asin(z) * 2.0;
		Value r = (*scanMsg)["ranges"];
	}
	return res;
}
/* namespace rosbridge */

}

