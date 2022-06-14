#include "EventContext.hpp"

EventContext::EventContext(int fd, EventType type, void* data)
: _eventIdent(fd)
, _eventType(type)
, _data(data) {
	this->setPipe(-1, -1);
}

std::string EventContext::eventTypeToString(EventType type) {
	switch (type) {
	case EV_Accept:
		return "EV_Accept";
	case EV_ProcessRequest:
		return "EV_ProcessRequest";
	case EV_DisposeConn:
		return "EV_DisposeConn";
	case EV_Request:
		return "EV_Request";
	case EV_Response:
		return "EV_Response";
	case EV_CGIParamBody:
		return "EV_CGIResponse";
	case EV_CGIResponse:
		return "EV_CGIResponse";
    case EV_SetVirtualServerErrorPage:
        return "EV_SetVirtualServerErrorPage";
    case EV_GETResponse:
        return "EV_GETResponse";
    case EV_POSTResponse:
        return "EV_POSTResponse";
	}
}

void EventContext::setPipe(int readPipe, int writePipe) {
	_pipe[0] = readPipe;
	_pipe[1] = writePipe;
}