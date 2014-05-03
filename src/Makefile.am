AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -O0 -g -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/lib

libSources = \
	lib/estacion/Auto.h \
	lib/estacion/constantes.h \
	lib/estacion/Tarea.h \
	lib/logging/Logger.cc \
	lib/logging/Logger.h \
	lib/logging/LoggerRegistry.cc \
	lib/logging/LoggerRegistry.h \
	lib/system/EventHandler.h \
	lib/system/IPCName.h \
	lib/system/Semaphore.cc \
	lib/system/Semaphore.h \
	lib/system/SharedArray.h \
	lib/system/SharedVariable.h \
	lib/system/SignalHandler.cc \
	lib/system/SignalHandler.h \
	lib/system/System.cc \
	lib/system/System.h \
	lib/utils/NonCopyable.h

productorAutosSources = \
	productor-autos/main.cc

simulacionSources = \
	simulacion/ArgParser.cc \
	simulacion/ArgParser.h \
	simulacion/main.cc

empleadoSources = \
	empleado/Empleado.cc \
	empleado/Empleado.h \
	empleado/main.cc

bin_PROGRAMS = \
	empleado/empleado \
	productor-autos/productor-autos \
	simulacion/simulacion

empleado_empleado_SOURCES = $(empleadoSources) $(libSources)
empleado_empleado_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/empleado
#empleado_empleado_LDADD =

productor_autos_productor_autos_SOURCES = $(productorAutosSources) $(libSources)
productor_autos_produtor_autos_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/productor-autos
#produtor_autos_produtor_autos_LDADD = 

simulacion_simulacion_SOURCES = $(simulacionSources) $(libSources)
simulacion_simulacion_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/simulacion
#simulacion_simulacion_LDADD = 
