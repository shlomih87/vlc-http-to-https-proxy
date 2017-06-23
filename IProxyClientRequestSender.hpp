/*
 * IProxyClientRequestSender.hpp
 *
 *  Created on: Jun 18, 2017
 *      Author: shlomihaguel
 */

#ifndef IPROXYCLIENTREQUESTSENDER_HPP_
#define IPROXYCLIENTREQUESTSENDER_HPP_

#include <string>

/**
 * Interface used by the proxy to connect to the server and
 * sending it requests
 */
class IProxyClientRequestSender
{

public:
	virtual bool connectToServer() = 0;

	virtual bool sendRequestToServer(const std::string& request) = 0;
};



#endif /* IPROXYCLIENTREQUESTSENDER_HPP_ */
