#include <cstddef>
#include <system/SignalHandler.h>

SignalHandler::SignalHandler ()
{
}

SignalHandler& SignalHandler::getInstance ()
{
	static SignalHandler instance;
	return instance;
}

EventHandler* SignalHandler::registrarHandler (int signum, EventHandler *eh)
{
	sigset_t mask;
	sigemptyset (&mask);
	return registrarHandler (signum, eh, mask);
}

EventHandler* SignalHandler::registrarHandler (int signum, EventHandler *eh, const sigset_t& mask)
{
	EventHandler *old_eh = SignalHandler::signal_handlers[signum];
	SignalHandler::signal_handlers[signum] = eh;

	struct sigaction sa;
	sa.sa_handler = SignalHandler::dispatcher;
	sa.sa_mask = mask; 
	sa.sa_flags = 0;
	sigaction (signum, &sa, 0);
	return old_eh;
}

void SignalHandler::dispatcher (int signum)
{
	if (SignalHandler::signal_handlers[signum] != NULL)
		SignalHandler::signal_handlers[signum]->handleSignal (signum);
}

int SignalHandler::removerHandler (int signum)
{
	SignalHandler::signal_handlers[signum] = NULL;
	return 0;
}

sigset_t SignalHandler::getProcMask ()
{
	sigset_t mask;
	sigprocmask (SIG_SETMASK, NULL, &mask);
	return mask;
}

EventHandler *SignalHandler::signal_handlers[NSIG] = { NULL };
