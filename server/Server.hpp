#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Log.hpp"
#include <string>
#include <vector>
#include <map>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>


class Server {
private:
	/* Server Configurations */

	/* Configuration related behaviors */

	/* HTTP Methods: GET, POST, DELETE */

public:
	void	process(/* Parsed Request */);
};

#endif
