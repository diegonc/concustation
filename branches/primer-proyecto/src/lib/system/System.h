#ifndef SYSTEM_H
#define SYSTEM_H

#include <exception>

#include <errno.h>
#include <string.h>
#include <unistd.h>

class SystemErrorException : public std::exception
{
	private:
		int _number;

	public:
		SystemErrorException () throw ();
		SystemErrorException (int err) throw ();

		virtual ~SystemErrorException () throw () { }

		virtual const char *what() const throw()
		{
			return strerror (this->_number);
		}

		int number () const { return _number; }
};

class System
{
	public:
		static pid_t spawn (const char *file, char* const argv[]);

		template <typename T>
		static void check (T err)
		{
			if (err == (T)-1) throw SystemErrorException ();
		}
};

#endif
