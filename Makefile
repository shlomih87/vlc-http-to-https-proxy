CC = g++
LIBS = -lboost_system -lssl -lcrypto -lpthread
FLAGS = --std=c++11 -g

all: ProxyServer

ProxyServer: Logger.o ProxySSLClientFactory.o ProxyHttpServer.o ProxySSLClient.o Main.o
	$(CC) -o $@ $(FLAGS) Logger.o ProxySSLClientFactory.o ProxyHttpServer.o ProxySSLClient.o Main.o $(LIBS)

ProxySSLClientFactory.o: IProxyClientRequestSender.hpp IProxyServerResponseReceiver.hpp
	$(CC) -c $(FLAGS) ProxySSLClientFactory.cpp

ProxySSLClient.o: ProxySSLClient.hpp
	$(CC) -c $(FLAGS) ProxySSLClient.cpp

ProxyHttpServer.o: ProxyHttpServer.hpp ProxySSLClient.hpp
	$(CC) -c $(FLAGS) ProxyHttpServer.cpp

Logger.o: Logger.hpp
	$(CC) -c $(FLAGS) Logger.cpp

Main.o: ProxyHttpServer.hpp
	$(CC) -c $(FLAGS) Main.cpp

clean:
	rm *.o ProxyServer


