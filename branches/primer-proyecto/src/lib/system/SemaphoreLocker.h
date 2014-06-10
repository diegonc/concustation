#ifndef SEMAPHORELOCKER_H
#define SEMAPHORELOCKER_H

#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <system/Semaphore.h>
#include <system/System.h>

class SemaphoreLocker
{
	Semaphore& sem;

	public:
		SemaphoreLocker (Semaphore& sem)
			: sem (sem)
		{
			int err = sem.wait ();
			System::check (err);
		}

		~SemaphoreLocker ()
		{
			try {
				sem.signal ();
			} catch (SystemErrorException& e) {
				Logger& logger = LoggerRegistry::getLogger ("SemaphoreLocker");
				logger << "Se atrapó una excepción al desbloquear el semaforo: "
				       << e.what ()
				       << Logger::endl;
			}
		}
};
#endif // SEMAPHORELOCKER_H
