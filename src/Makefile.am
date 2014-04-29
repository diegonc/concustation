AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -O0 -g -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/lib

libSources = \
	lib/estacion/constantes.h \
	lib/system/IPCName.h \
	lib/system/Semaphore.cc \
	lib/system/Semaphore.h \
	lib/system/System.cc \
	lib/system/System.h \
	lib/utils/NonCopyable.h

productorAutosSources = \
	productor-autos/main.cc

bin_PROGRAMS = \
	productor-autos/productor-autos

productor_autos_productor_autos_SOURCES = $(productorAutosSources) $(libSources)
productor_autos_produtor_autos_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/productor-autos
#produtor_autos_produtor_autos_LDADD = 
