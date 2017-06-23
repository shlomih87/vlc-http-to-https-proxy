/*
 * Main.cpp
 *
 *  Created on: Jun 18, 2017
 *      Author: shlomihaguel
 */

#include <iostream>
#include <string>
#include <thread>

#include "ProxyHttpServer.hpp"

static const std::string mediaPlayerCommand = "vlc";

static const std::string mediaPlayerUrl = "http://127.0.0.1/liveorigin/stream4/playlist.m3u8";

static void startMediaPlayer()
{
	system((mediaPlayerCommand + " " + mediaPlayerUrl + "& > /dev/null 2>&1").c_str());
}

int main()
{
	try
	{
		boost::asio::io_service ioService;

		ProxyHttpServer proxyHttpServer(ioService, 80,
				"wowza-cloudfront.streamroot.io");

		startMediaPlayer();

		ioService.run();

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;

}

