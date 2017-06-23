/*
 * ProxyHttpServer.hpp
 *
 *  Created on: Jun 17, 2017
 *      Author: shlomihaguel
 */

#ifndef PROXYHTTPSERVER_HPP_
#define PROXYHTTPSERVER_HPP_

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "IProxyClientRequestSender.hpp"
#include "IProxyServerResponseReceiver.hpp"
#include "Logger.hpp"


using boost::asio::ip::tcp;

/*
 * The classes manages a specific connection between the client and the
 * proxy's server
 */
class ProxyHttpSession : public IProxyServerResponseReceiver
{

public:
	ProxyHttpSession(boost::asio::io_service& io_service,
			std::string serverHostName);

	void start();

	tcp::socket& getSocket();

	/**
	 * Handler of incoming data from the proxy's client
	 */
	void clientRequestHandler(const boost::system::error_code& error,
			size_t bytes_transferred);

	/*
	 * Callback for treating the response from the proxy's server
	 */
	void receiveServerResponse(const char* response_data,
			int size_in_bytes);

	/*
	 * Sets the IProxyClientRequestSender object used for sending incoming data to
	 * the proxy's server (m_pClientRequestSender)
	 */
	void setClientRequestSender(IProxyClientRequestSender *pClientRequestSender);

	void logMsg(const std::string& msg);

	void printRequest(const char* data, int size_in_bytes);

	/*
	 * m_pClientRequestSender is freed here.
	 * If the client ends its connection, the connection with the server
	 * should be ended as well
	 */
	~ProxyHttpSession();

private:
	tcp::socket m_sock;

	static const int MAX_DATA_LENGTH = 1024;

	char m_data[MAX_DATA_LENGTH];
	//char data_send[MAX_DATA_LENGTH];

	IProxyClientRequestSender *m_pClientRequestSender;

	std::string m_DestServerHostName;

	static std::string s_localHostIp;

	Logger* m_pLogger;
};

/*
 * The class manages the server that listens to incoming data from
 * the client
 */
class ProxyHttpServer
{

public:
	ProxyHttpServer(boost::asio::io_service& io_service, short port,
			const std::string& proxyDestHostName);

	/**
	 * Handler for accepting a new connection to the proxy's server
	 */
	void acceptClientConnection(ProxyHttpSession* newSession,
			const boost::system::error_code& error);


	void logMsg(const std::string& msg);


private:
	boost::asio::io_service& m_ioService;
	tcp::acceptor m_newSessionAcceptor;
	Logger* m_pLogger;
	std::string m_proxyDestHostName;
	ProxyHttpSession *m_pLastActiveSession;
};


#endif /* PROXYHTTPSERVER_HPP_ */
