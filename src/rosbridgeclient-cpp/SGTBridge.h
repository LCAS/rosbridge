/*
 * SGTBridge.h
 *
 *  Created on: 9 Jul 2013
 *      Author: marc
 */

#ifndef SGTBRIDGE_H_
#define SGTBRIDGE_H_

#include "RosbridgeClient.h"

namespace rosbridge {

class SGTBridge: public rosbridge::RosbridgeClient {
public:
	SGTBridge(std::string host, std::string port);
	virtual ~SGTBridge();
};

} /* namespace rosbridge */
#endif /* SGTBRIDGE_H_ */
