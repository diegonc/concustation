#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class EventHandler
{
	public :
		virtual ~EventHandler () {}
		virtual void handleSignal (int signum) = 0;

};

#endif /* EVENTHANDLER_H */
