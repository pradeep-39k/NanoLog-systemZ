/*
 * Logger.hpp
 *
 *  Created on: Jun 6, 2012
 *      Author: Sumit Beniwal
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <system-z/utils/Time.hpp>

namespace wallsoft { 


// TODO: Improve logging with multiple verbose levels. And write to some file.
class Logger{

public:

	static void log(const std::string& str){

		int64_t timestamp = Time::getTimestamp();

		std::cout << "\n[";
		Time::printTimestamp(std::cout, timestamp);
		std::cout << " | " << timestamp << "] "<< str << std::flush;
	}


	static void log(const char* str){

		int64_t timestamp = Time::getTimestamp();

		std::cout << "\n[";
		Time::printTimestamp(std::cout, timestamp);
		std::cout << " | " << timestamp << "] "<< str << std::flush;
	}

};


}
#endif /* LOGGER_HPP_ */
