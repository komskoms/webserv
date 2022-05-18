#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include "Log.hpp"
#include <string>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "Request.hpp"
#include "Response.hpp"

//  General coonection handler, from generation communication.
//   - TODO
//      Connection should handle the receiving and transmiting without malfunction.
//      소켓이 수신 결과를 Request객체로 저장할 수 있어야 함.
//      소켓이 Response객체를 이용해 송신을 처리할 수 있어야 함.
//   - Member Variables
//      _client
//      _ident
//      _addr
//      _port
//      _request
//   - Methods
class Connection {
public:
    Connection(int port);

    Connection* acceptClient();
    ReturnCaseOfRecv receive();
    void transmit();
    void addKevent(int kqueue, int filter, void* udata);

    bool isclient() { return this->_client; };
    int getIdent() { return this->_ident; };
    std::string getAddr() { return this->_addr; };
    int getPort() { return this->_port; };
    const Request& getRequest() const { return this->_request; };
    void setResponse();

private:
    bool _client;
    int _ident;
    std::string _addr;
    int _port;
    Request _request;
    Response _response;

    Connection(int ident, std::string addr, int port);

    void newSocket();
    void bindSocket();
    void listenSocket();
};

#endif  // CONNECTION_HPP_
