#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

/**
 * http://en.wikibooks.org/wiki/More_C++_Idioms/Non-copyable_Mixin
 */
class NonCopyable
{
	protected:
		NonCopyable () {}
		~NonCopyable () {} /// Protected non-virtual destructor
	private: 
		NonCopyable (const NonCopyable &);
		NonCopyable & operator = (const NonCopyable &);
};

#endif
