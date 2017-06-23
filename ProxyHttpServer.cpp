/*
 * ProxyHttpServer.cpp
 *
 *  Created on: Jun 18, 2017
 *      Author: shlomihaguel
 */

#include "ProxyHttpServer.hpp"
#include "ProxySSLClientFactory.hpp"

#include <iostream>

std::string ProxyHttpSession::s_localHostIp = "127.0.0.1";

ProxyHttpSession::ProxyHttpSession(boost::asio::io_service& io_service,
		std::string serverHostName)
:m_sock(io_service),
 m_DestServerHostName(serverHostName),
m_pLogger(Logger::getLogger())
{
}

tcp::socket& ProxyHttpSession::getSocket()
{
	return m_sock;
}


void ProxyHttpSession::start()
{
	//Asynchronously reading client input
	m_sock.async_read_some(boost::asio::buffer(m_data, MAX_DATA_LENGTH),
			boost::bind(&ProxyHttpSession::clientRequestHandler, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void ProxyHttpSession::clientRequestHandler(const boost::system::error_code& error,
		size_t bytes_transferred)
{
	if(!error)
	{
		if(bytes_transferred > 0)
		{
			//Printing request
			printRequest(m_data, bytes_transferred);
			auto data_str = std::string(m_data, bytes_transferred);
			/* Making sure the string has enough space allocated in order to
			  replace the Host HTTP header */
			data_str.resize(data_str.length() + m_DestServerHostName.length());
			auto host_pos = data_str.find(s_localHostIp);
			if (host_pos != std::string::npos)
			{
				data_str.replace(host_pos, s_localHostIp.length(), m_DestServerHostName);
			}

			if(!m_pClientRequestSender->sendRequestToServer(data_str))
			{
				/*If there's a failure with the connection to server,
				 * both the session with the server and with the client
				 * are killed. The client should start a new one.
				 */
				delete this;
				return;
			}
		}

		//Asynchronously reading the next input from client
		m_sock.async_read_some(boost::asio::buffer(m_data, MAX_DATA_LENGTH),
				boost::bind(&ProxyHttpSession::clientRequestHandler, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		//Connection with the client has ended. Deleting session object
		//logMsg("Connection with the client has ended");
		delete this;
	}
}

void ProxyHttpSession::receiveServerResponse(const char* response_data,
		int size_in_bytes)
{
	char sent_data[size_in_bytes];
	memcpy(sent_data,response_data,size_in_bytes);
	boost::asio::write(getSocket(),
        boost::asio::buffer(sent_data, size_in_bytes));
}


void ProxyHttpSession::setClientRequestSender(IProxyClientRequestSender *pClientRequestSender)
{
	m_pClientRequestSender = pClientRequestSender;
}

void ProxyHttpSession::logMsg(const std::string& msg)
{
	if(m_pLogger)
		m_pLogger->sentToLog(msg);
}

void ProxyHttpSession::printRequest(const char* data, int size_in_bytes)
{
	std::cout << "Got a request from the client:" << std::endl;
	write(STDOUT_FILENO, m_data, size_in_bytes);
	std::cout << std::endl;
}

ProxyHttpSession::~ProxyHttpSession()
{
	delete m_pClientRequestSender;
}



ProxyHttpServer::ProxyHttpServer(boost::asio::io_service& io_service,
		short port, const std::string& proxyDestHostName)
: m_ioService(io_service),
  m_newSessionAcceptor(io_service, tcp::endpoint(tcp::v4(), port)),
  m_proxyDestHostName(proxyDestHostName),
  m_pLogger(Logger::getLogger()),
  m_pLastActiveSession(nullptr)
{
	//The accepted new session will be created in newSession
	auto newSession = new ProxyHttpSession(m_ioService, m_proxyDestHostName);
	/* We cause the asynchronous call to acceptClientConnection() when new
	  connection is accepted */
	m_newSessionAcceptor.async_accept(newSession->getSocket(),
			boost::bind(&ProxyHttpServer::acceptClientConnection, this, newSession,
					boost::asio::placeholders::error));
}

void ProxyHttpServer::acceptClientConnection(ProxyHttpSession* newSession,
		const boost::system::error_code& error)
{
    if (!error)
    {
      auto pSSLClient = ProxySSLClientFactory::getNewClientRequestSender(m_proxyDestHostName, 443, newSession);
      if(!pSSLClient->connectToServer())
      {
    	  logMsg("Proxy SSL client failed to connect");
    	  return;
      }
      newSession->start();
      newSession->setClientRequestSender(pSSLClient);
      m_pLastActiveSession = newSession;


      newSession = new ProxyHttpSession(m_ioService, m_proxyDestHostName);

      //Asynchronously accpeting new connection from client
      m_newSessionAcceptor.async_accept(newSession->getSocket(),
        boost::bind(&ProxyHttpServer::acceptClientConnection, this, newSession,
          boost::asio::placeholders::error));
    }
    else
    {
    	std::cout << "Error in accepting new connection" <<  std::endl;
    	delete newSession;
    }
}


void ProxyHttpServer::logMsg(const std::string& msg)
{
	if(m_pLogger)
		m_pLogger->sentToLog(msg);
}


