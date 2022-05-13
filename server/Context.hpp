#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

class Request;

class Context {
private:
	int			_clientSocket;
	Request*	_request;

public:
	int			getClientSocket();
	Request*	getRequest();
};

#endif