#ifndef LOG_HPP_
#define LOG_HPP_

#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

/**
 * @brief Parsed value of the HTTP Headers.
 * 
 */

class Log {
private:

public:
	static void Verbose(const char* format, ...);
};


#endif
