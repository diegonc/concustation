#ifndef SEMAPHORELOCKER_H
#define SEMAPHORELOCKER_H

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
			sem.signal ();
		}
};
#endif // SEMAPHORELOCKER_H
