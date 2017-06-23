/*
 * ProxySSLClient.hpp
 *
 *  Created on: Jun 17, 2017
 *      Author: shlomihaguel
 */

#ifndef PROXYSSLCLIENT_HPP_
#define PROXYSSLCLIENT_HPP_

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "Logger.hpp"
#include "IProxyClientRequestSender.hpp"
#include "IProxyServerResponseReceiver.hpp"

/*
 * The class responsible for the communication with the SSL server
 */
class ProxySSLClient : public IProxyClientRequestSender
{

public:
	ProxySSLClient(const std::string& hostName, short port,
			IProxyServerResponseReceiver* pResponseReceiver);

	/*
	 * Doing the SSL handshake with the server
	 */
    bool connectToServer();

    bool sendRequestToServer(const std::string& request);

    /*
     * Retrieves the response from the server and forwards it to the
     * proxy's client (m_pResponseReceiver)
     */
    void getServerResponseAndSendToReceiver();

    void logMsg(const std::string& msg);

    ~ProxySSLClient();

private:
    /*
     * Forwards the server's response to the proxy's client
     */
    void sendServerResponseToReceiver(const char* data, int size_in_bytes);

    /*
     * Init the OpenSSL library, in the first usage
     */
    bool initSSLLibrary();

    static const int MAX_DATA_LENGTH = 1024;

    char m_data[MAX_DATA_LENGTH];

    std::string m_hostName;
    int m_port;

    SSL_CTX* m_pCtx;
    SSL* m_pSSL;
    int m_socket_;
    IProxyServerResponseReceiver* m_pResponseReceiver;
    Logger* m_pLogger;
};

#endif /* PROXYSSLCLIENT_HPP_ */
