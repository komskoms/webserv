#ifndef EVENTCONTEXT_HPP_
#define EVENTCONTEXT_HPP_

#include <string>

class EventContext {
public:
	enum EventType {
        EV_SetVirtualServerErrorPage,
		EV_Accept,
		EV_Request,
		EV_ProcessRequest,
		EV_CGIParamBody,
		EV_CGIResponse,
        EV_GETResponse,
        EV_POSTResponse,
		EV_Response,
		EV_DisposeConn,
	};
	enum EventResult {
		ER_Done,
		ER_Remove,
		ER_Continue,
		ER_NA,
	};

	EventContext(int fd, EventType type, void* data);

	int getIdent() { return _eventIdent; };
	EventType getEventType() { return _eventType; };
	std::string eventTypeToString(EventType type);
	std::string getEventTypeToString() { return this->eventTypeToString(_eventType); };
	void* getData() { return _data; };
	int getReadPipe() { return _pipe[0]; };
	int getWritePipe() { return _pipe[1]; };
	// void setData(void* data) { };
	void setPipe(int readPipe, int writePipe);

private:
	int _eventIdent;
	EventType	_eventType;
	int _pipe[2];
	void* _data;
};

#endif
