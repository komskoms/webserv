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

	void addEvent(int filter, int fd, EventContext::EventType type, void* data);
	void addEvent(int filter, int fd, EventContext::EventType type, void* data, int pipe[2]);
	void removeEvent(int filter, EventContext* context);
	void addUserEvent(int fd, EventContext::EventType type, void* data);
	int checkEvent(struct kevent* eventlist);
    void addTimeoutEvent(int fd);
    void resetTimeoutEvent(int fd);

private:
	const int _kqueue;
	const int _maxEvent;

	enum { MaxEventNumber = 20 };
};

#endif
