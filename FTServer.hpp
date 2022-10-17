#ifndef FTSERVER_HPP_
#define FTSERVER_HPP_

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
#include <utility>
#include "Log.hpp"
#include "VirtualServer.hpp"
#include "Connection.hpp"
#include "VirtualServerConfig.hpp"
#include "EventHandler.hpp"

// NOTE port, server_name only one
// vector<string>? for multiple server name
struct ServerConfigKey {
    std::string _port;
    std::vector<std::string> _server_name;
    bool operator<(const ServerConfigKey s) const {
        return ((this->_port < s._port) || (this->_server_name < s._server_name));
    }
};

// Manage multiple servers (like nginx)
//  - Member variables  
//      _defaultConfigs: config file에서 파싱해서 정리한 config 컨테이너(vector), 서버마다 속성값 다르기에 구분
//      _vVirtualServers: 
//      _mConnection
//      _kqueue
//      _alive
//  - Methods
//      init: Read and parse configuration file to initialize server. 
class FTServer {
public:
    FTServer();
    ~FTServer();

    void init();
    void initializeVirtualServers();
    void initParseConfig(std::string configfile);
    void initializeConnection(std::set<port_t>&  ports);

    VirtualServer& getTargetVirtualServer(Connection& connection);
    void eventAcceptConnection(int ident);
    void run();

    class NOTOPENFILEERROR : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "not open file";
        }
    };
private:
    typedef std::vector<VirtualServer*>             VirtualServerVec;
    typedef std::map<int, Connection*>           ConnectionMap;
    typedef std::map<int, Connection*>::iterator ConnectionMapIter;
    typedef std::vector<VirtualServerConfig *>  VirtualServerConfigVec;
    typedef std::vector<VirtualServerConfig *>::iterator  VirtualServerConfigIter;

    VirtualServerConfigVec _defaultConfigs;
    VirtualServerVec       _vVirtualServers;
    ConnectionMap       _mConnection;
    std::map<port_t, VirtualServer*> _defaultVirtualServers;
    bool            _alive;
    EventHandler _eventHandler;

    VirtualServer* makeVirtualServer(VirtualServerConfig* serverConf);
    void eventAcceptConnection(Connection* connection);
    void handleUserFlaggedEvent(struct kevent event);

    EventContext::EventResult driveThisEvent(EventContext* context, int filter);
    void runEachEvent(struct kevent event);
    void eventProcessRequest(EventContext* context);

    EventContext::EventResult eventSetVirtualServerErrorPage(EventContext& context);
    EventContext::EventResult eventGETResponse(EventContext& context);
    EventContext::EventResult eventPOSTResponse(EventContext& context);
    EventContext::EventResult eventTimeout(EventContext* context);
    void printParseResult();
};

#endif  // FTSERVER_HPP_
