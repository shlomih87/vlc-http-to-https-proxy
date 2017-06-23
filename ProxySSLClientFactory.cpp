/*
 * ProxySSLClientFactory.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: shlomihaguel
 */

#include "ProxySSLClientFactory.hpp"
#include "ProxySSLClient.hpp"

/**
 * Factory class for creating an instance of the client side of the proxy -
 * the one communicating with the server
 */
IProxyClientRequestSender* ProxySSLClientFactory::getNewClientRequestSender(
		const std::string& hostName,
		short port,
		IProxyServerResponseReceiver* pResponseReceiver)
{
	return new ProxySSLClient(hostName, port, pResponseReceiver);
}

