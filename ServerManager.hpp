#ifndef SERVERMANAGER_HPP_
#define SERVERMANAGER_HPP_

#include "Log.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "ServerConfig.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <exception>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>


class ServerManager {
public:
	ServerManager();
    ~ServerManager();

	/**
	 * @brief Read and parse configuration file to initialize server.
	 * 
	 */
    void init();
    void setUpServer();

	void		initParseConfig(std::string configfile);
	// void    errorParsing(std::string cause) {}

	void		initializeServers();
	void		initializeSocket(int ports[], int size);
	void		run();

    Server& getTargetServer(Socket& socket);

private:
	typedef		std::vector<Server*>					ServerVec;
	typedef		std::map<int, Socket*>				SocketMap;
	typedef		std::map<int, Socket*>::iterator	SocketMapIter;
	typedef		std::set<ServerConfig *>			ServerConfigSet;

	ServerConfigSet			_defaultConfigs;    // 서버마다 속성값 다르기에 구분
	ServerVec				_vServers;
	SocketMap				_mSocket;
	int						_kqueue;
	bool					_alive;

	/**
	 * @brief Generate new Server object as configurated and register to the Server container.
	 * 
	 */
	void		makeServer(ServerConfig* serverConf);


	void		clientAccept(Socket* socket);
	void		read(Socket* socket);
	void		write(Socket* socket);

	Server*	selectServer(/* Request Object */);

};

#endif
