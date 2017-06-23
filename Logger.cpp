/*
 * Logger.cpp
 *
 *  Created on: Jun 18, 2017
 *      Author: shlomihaguel
 */

#include <iostream>

#include "Logger.hpp"

Logger* Logger::s_logger = nullptr;

Logger::Logger()
{
}

void Logger::sentToLog(const string& logMsg)
{
	std::cout << logMsg << std::endl;
}

Logger* Logger::getLogger()
{
	if(!s_logger)
		s_logger = new Logger;

	return s_logger;
}




