/*
 * IProxyServerResponseReceiver.hpp
 *
 *  Created on: Jun 18, 2017
 *      Author: shlomihaguel
 */

#ifndef IPROXYSERVERRESPONSERECEIVER_HPP_
#define IPROXYSERVERRESPONSERECEIVER_HPP_

/*
 * Interface used by the proxy's client in order to send response to the
 * proxy's server (which should forward it to the client)
 */
class IProxyServerResponseReceiver
{

public:
	virtual void receiveServerResponse(const char* response_data,
			int size_in_bytes) = 0;

};



#endif /* IPROXYSERVERRESPONSERECEIVER_HPP_ */
