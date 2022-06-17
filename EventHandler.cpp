#include "EventHandler.hpp"
#include "constant.hpp"

EventHandler::EventHandler()
: _kqueue(kqueue())
, _maxEvent(MaxEventNumber)
, _connectionDeleted(false) {
	if (_kqueue < 0)
		throw std::logic_error("Cannot create EventHandler.");
}

EventHandler::~EventHandler() {
	close(_kqueue);
}

// Add new event on Kqueue
//  - Parameters
//      kqueue: FD number of Kqueue
//      filter: filter value for Kevent
//      udata: user data (optional)
//  - Return(none)
EventContext* EventHandler::addEvent(int filter, int fd, EventContext::EventType type, void* data) {
	struct kevent ev;
	EventContext* context = new EventContext(fd, type ,data);

    EV_SET(&ev, fd, filter, EV_ADD | EV_ENABLE, 0, 0, context);
    if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0) {
		delete context;
		Log::warning("Event add Failure. [%d] [%s].", fd, type);
        throw std::runtime_error(strerror(errno));
	}
	return context;
}
// Add new event on Kqueue(CGI case)
EventContext* EventHandler::addEvent(int filter, int fd, EventContext::EventType type, void* data, int pipe[2]) {
	struct kevent ev;
	EventContext* context = new EventContext(fd, type ,data);

	context->setPipe(pipe[0], pipe[1]);
    EV_SET(&ev, fd, filter, EV_ADD | EV_ENABLE, 0, 0, context);
    if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0) {
		delete context;
		Log::warning("Event add Failure. [%d] [%d].", fd, type); // todo stupid
        throw std::runtime_error(strerror(errno));
	}
	return context;
}

// Remove existing event on Kqueue
//  - Parameters
//      kqueue: FD number of Kqueue
//      filter: filter value for Kevent to remove
//      udata: user data (optional)
//  - Return(none)
void EventHandler::removeEvent(int filter, EventContext* context) {
	struct kevent ev;
	EventContext::EventType eventType = context->getEventType();
	int fd = context->getIdent();

	if (eventType == EventContext::EV_CGIParamBody ||
		eventType == EventContext::EV_CGIResponse) {
        close(context->getReadPipe());
        close(context->getWritePipe());
		Log::verbose("CGI pipe closed. [%s] [%d] [%d]", context->getEventTypeToString().c_str(), context->getReadPipe(), context->getWritePipe());
	} else {
		EV_SET(&ev, fd, filter, EV_DELETE, 0, 0, context);
		if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0)
			throw std::runtime_error("RemoveEvent Failed.");
	}
    if (context->getEventType() != EventContext::EV_Request)
        delete context;
}

// Add custom event on Kqueue (triggered just for 1 time)
//  - Parameters
//      context: EventContext for event
//  - Return(none)
EventContext* EventHandler::addUserEvent(int fd, EventContext::EventType type, void* data) {
	struct kevent ev;
	EventContext* context = new EventContext(fd, type ,data);

    EV_SET(&ev, fd, EVFILT_USER, EV_ADD | EV_ONESHOT, NOTE_TRIGGER, 0, context);
    if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0)
        throw std::runtime_error("AddEvent(Oneshot flagged) Failed.");
	return context;
}
// Check a number of event in kqueue
int EventHandler::checkEvent(struct kevent* eventlist) {
	return kevent(this->_kqueue, NULL, 0, eventlist, _maxEvent, NULL);
}

// Add a Timeout event
void EventHandler::addTimeoutEvent(EventContext* context) {
    struct kevent ev;
	int fd = context->getIdent();

    EV_SET(&ev, fd, EVFILT_TIMER, EV_ADD | EV_ONESHOT, 0, TIMEOUT, context);
    if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0)
        throw std::runtime_error("AddEvent(timeout) Failed.");
}
// Add an event to reset a timeout event
void EventHandler::resetTimeoutEvent(EventContext* context) {
    struct kevent ev;
	int fd = context->getIdent();

    EV_SET(&ev, fd, EVFILT_TIMER, EV_DELETE, 0, TIMEOUT, context);
    if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0)
        throw std::runtime_error("AddEvent(timeout reset) Failed.");
    EV_SET(&ev, fd, EVFILT_TIMER, EV_ADD | EV_ONESHOT, 0, TIMEOUT, context);
    if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0)
        throw std::runtime_error("AddEvent(timeout reset) Failed.");
}
// Add an event to delete a timeout event
void EventHandler::deleteTimeoutEvent(int fd) {
    struct kevent ev;

    EV_SET(&ev, fd, EVFILT_TIMER, EV_DELETE, 0, TIMEOUT, 0);
    if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0)
        throw std::runtime_error("AddEvent(timeout delete) Failed.");
}
