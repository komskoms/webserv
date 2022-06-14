#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

#include <unistd.h>
#include <sys/event.h>
#include <exception>
#include "Log.hpp"
#include "EventContext.hpp"

class EventHandler {
public:
	enum {
		CONNECTION,
		CGIResponse,
	};

	EventHandler();
	~EventHandler();

	int getKqueue() { return _kqueue; };
	int getMaxEvent() { return _maxEvent; };
	bool isConnectionDeleted() { return _connectionDeleted; };
	void setConnectionDeleted(bool set) { _connectionDeleted = set; };

	EventContext* addEvent(int filter, int fd, EventContext::EventType type, void* data);
	EventContext* addEvent(int filter, int fd, EventContext::EventType type, void* data, int pipe[2]);
	void removeEvent(int filter, EventContext* context);
	EventContext* addUserEvent(int fd, EventContext::EventType type, void* data);
	int checkEvent(struct kevent* eventlist);
    void addTimeoutEvent(EventContext* context);
    void resetTimeoutEvent(EventContext* context);
    void deleteTimeoutEvent(int fd);

private:
	const int _kqueue;
	const int _maxEvent;
	bool _connectionDeleted;

	enum { MaxEventNumber = 20 };
};

#endif
