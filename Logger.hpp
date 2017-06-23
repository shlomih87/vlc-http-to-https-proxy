/*
 * Logger.hpp
 *
 *  Created on: Jun 17, 2017
 *      Author: shlomihaguel
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>

using namespace std;

class Logger
{

public:
	void sentToLog(const string& logMsg);

	static Logger* getLogger();

private:
	Logger();

	static Logger* s_logger;
};




#endif /* LOGGER_HPP_ */
