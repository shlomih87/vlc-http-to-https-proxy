/*
 * ProxySSLClientFactory.hpp
 *
 *  Created on: Jun 21, 2017
 *      Author: shlomihaguel
 */

#ifndef PROXYSSLCLIENTFACTORY_HPP_
#define PROXYSSLCLIENTFACTORY_HPP_

#include <string>
#include "IProxyClientRequestSender.hpp"
#include "IProxyServerResponseReceiver.hpp"

/*
 * Interface Used by the proxy's server to send request data to the proxy's
 * client (which is responsible for forwarding it to the server)
 */
class ProxySSLClientFactory
{

public:
	static IProxyClientRequestSender* getNewClientRequestSender(
			const std::string& hostName,
			short port,
			IProxyServerResponseReceiver* pResponseReceiver);

};



#endif /* PROXYSSLCLIENTFACTORY_HPP_ */
