#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include <signal.h>

#include <system/EventHandler.h>

class SignalHandler
{
	private :
		static EventHandler *signal_handlers[NSIG];

		SignalHandler (void);
		static void dispatcher (int signum);

	public :
		static SignalHandler& getInstance ();
		
		EventHandler* registrarHandler (int signum, EventHandler *eh);
		EventHandler* registrarHandler (int signum, EventHandler *eh, const sigset_t& mask);
		int removerHandler (int signum);

		static sigset_t getProcMask ();
		static sigset_t setProcMask (const sigset_t& mask);
};

#endif /* SIGNALHANDLER_H */
