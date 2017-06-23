/*
 * ProxySSLClient.cpp
 *
 *  Created on: Jun 17, 2017
 *      Author: shlomihaguel
 */

#include "ProxySSLClient.hpp"

#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

ProxySSLClient::ProxySSLClient(const std::string& hostName, short port,
		IProxyServerResponseReceiver* pResponseReceiver):
		m_hostName(hostName), m_port(port), m_pResponseReceiver(pResponseReceiver),
		m_pLogger(Logger::getLogger()), m_socket_(-1), m_pCtx(NULL), m_pSSL(NULL)
{
}

bool ProxySSLClient::initSSLLibrary()
{
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();

	if(SSL_library_init() < 0)
	{
		logMsg("Error - SSL_library_init() failed");
		return false;
	}

	return true;
}

bool ProxySSLClient::connectToServer()
{
	static bool firstTime = true;

	//In the first time we init the SSL library
	if(firstTime)
	{
		if(!initSSLLibrary())
		{
			logMsg("Error - TInit SSL library");
			return false;
		}
		firstTime = false;
	}

	//SSL method is TLS v1.2
	const SSL_METHOD *pMethod = TLSv1_2_method();
	if(pMethod == NULL)
	{
		logMsg("Error - TLSv1_2_method() failed");
		return false;
	}

	m_pCtx = SSL_CTX_new(pMethod);
	if(m_pCtx == NULL)
	{
		logMsg("Error - SSL_CTX_new() failed");
		return false;
	}

	struct hostent *host = gethostbyname(m_hostName.c_str());
	if(host == NULL)
	{
		logMsg("Error - gethostbyname() failed");
		return false;
	}
	//Creating linux socket on which SSL will be opened
	m_socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if(m_socket_ == -1)
	{
		logMsg("Error - socket() failed");
		return false;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);

	//Creating a tcp session with the server
	if (connect(m_socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		logMsg("Failed to connect to server");
		return false;
	}

	m_pSSL = SSL_new(m_pCtx);
	if(m_pSSL == NULL)
	{
		logMsg("SSL_new() failed");
		return false;
	}
	if(SSL_set_fd(m_pSSL, m_socket_) == 0)
	{
		logMsg("SSL_set_fd() failed");
		return false;
	}
	//Add the host name to the TLS Client Hello request (needed for TLS v1.2)
	if(SSL_set_tlsext_host_name(m_pSSL, m_hostName.c_str()) != 1)
	{
		logMsg("SSL_set_tlsext_host_name() failed");
		return false;
	}

	//Creating the SSL handshake with the server
	if(SSL_connect(m_pSSL) != 1)
	{
		logMsg("SSL_connect() failed");
		return false;
	}

	//free(pMethod);

	return true;
}

bool ProxySSLClient::sendRequestToServer(const std::string& request)
{
    if(SSL_write(m_pSSL, request.c_str(), request.length()) <= 0)
    {
    	logMsg("Failed to forward request to server");
    	return false;
    }

    getServerResponseAndSendToReceiver();
    return true;
}

void ProxySSLClient::getServerResponseAndSendToReceiver()
{
	auto bytes_read = SSL_read(m_pSSL, m_data, sizeof(m_data));

	//Looping until the full HTTP response is got from the server
	while(bytes_read > 0)
	{
		sendServerResponseToReceiver(m_data, bytes_read);
		bytes_read = SSL_read(m_pSSL, m_data, sizeof(m_data));
    }
	/* If bytes_read <= 0, either an error occurred or connection has ended.
	 * Since the connection is non-persistent HTTP, the client will initiate
	 * a new connection by itself upon a new request.
	 * Thus, there's no return value to indicate the status of the connection
	 */

}


void ProxySSLClient::sendServerResponseToReceiver(const char* data,
		int size_in_bytes)
{
	if(m_pResponseReceiver != nullptr)
		m_pResponseReceiver->receiveServerResponse(data, size_in_bytes);
}

ProxySSLClient::~ProxySSLClient()
{
	if(m_pSSL != NULL)
		SSL_free(m_pSSL);

	if(m_socket_ != -1)
		close(m_socket_);

	if(m_pCtx != NULL)
		SSL_CTX_free(m_pCtx);
}


void ProxySSLClient::logMsg(const std::string& msg)
{
	if(m_pLogger)
		m_pLogger->sentToLog(msg);
}




