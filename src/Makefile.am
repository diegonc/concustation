AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -O0 -g -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/lib

libSources = \
	lib/estacion/constantes.h \
	lib/estacion/Configuracion.h \
	lib/estacion/ListaEntero.cc \
	lib/estacion/ListaEntero.h \
	lib/estacion/OpJefe.h \
	lib/estacion/OpSurtidores.h \
	lib/estacion/OpCaja.h \
	lib/estacion/Tarea.h \
	lib/logging/Logger.cc \
	lib/logging/Logger.h \
	lib/logging/LoggerRegistry.cc \
	lib/logging/LoggerRegistry.h \
	lib/system/EventHandler.h \
	lib/system/FifoFilesystemObject.cc \
	lib/system/FifoFilesystemObject.h \
	lib/system/File.cc \
	lib/system/File.h \
	lib/system/IPCName.h \
	lib/system/MessageQueue.h \
	lib/system/Semaphore.cc \
	lib/system/Semaphore.h \
	lib/system/SemaphoreLocker.h \
	lib/system/SharedArray.h \
	lib/system/SharedVariable.h \
	lib/system/SignalHandler.cc \
	lib/system/SignalHandler.h \
	lib/system/System.cc \
	lib/system/System.h \
	lib/utils/NonCopyable.h

productorAutosSources = \
	productor-autos/ArgParser.cc \
	productor-autos/ArgParser.h \
	productor-autos/ProductorAutos.cc \
	productor-autos/ProductorAutos.h \
	productor-autos/main.cc

simulacionSources = \
	simulacion/ArgParser.cc \
	simulacion/ArgParser.h \
	simulacion/Simulacion.cc \
	simulacion/Simulacion.h \
	simulacion/main.cc

empleadoSources = \
	empleado/ArgParser.cc \
	empleado/ArgParser.h \
	empleado/Empleado.cc \
	empleado/Empleado.h \
	empleado/main.cc
	
surtidoresSources = \
	surtidores/ArgParser.cc \
	surtidores/ArgParser.h \
	surtidores/Surtidores.cc \
	surtidores/Surtidores.h \
	surtidores/main.cc
	
cajaSources = \
	caja/ArgParser.cc \
	caja/ArgParser.h \
	caja/Caja.cc \
	caja/Caja.h \
	caja/main.cc

jefeSources = \
	jefe/ArgParser.cc \
	jefe/ArgParser.h \
	jefe/Jefe.cc \
	jefe/Jefe.h \
	jefe/main.cc
	
administradorSources = \
	administrador/ArgParser.cc \
	administrador/ArgParser.h \
	administrador/Administrador.cc \
	administrador/Administrador.h \
	administrador/main.cc

bin_PROGRAMS = \
	empleado/empleado \
	surtidores/surtidores \
	caja/caja \
	jefe/jefe \
	productor-autos/productor-autos \
	administrador/administrador \
	simulacion/simulacion

empleado_empleado_SOURCES = $(empleadoSources) $(libSources)
empleado_empleado_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/empleado
#empleado_empleado_LDADD =

surtidores_surtidores_SOURCES = $(surtidoresSources) $(libSources)
surtidores_surtidores_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/surtidores
#surtidores_surtidores_LDADD =

caja_caja_SOURCES = $(cajaSources) $(libSources)
caja_caja_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/caja
#caja_caja_LDADD =

jefe_jefe_SOURCES = $(jefeSources) $(libSources)
jefe_jefe_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/jefe
#jefe_jefe_LDADD =

productor_autos_productor_autos_SOURCES = $(productorAutosSources) $(libSources)
productor_autos_productor_autos_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/productor-autos
#produtor_autos_produtor_autos_LDADD =

administrador_administrador_SOURCES = $(administradorSources) $(libSources)
administrador_administrador_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/administrador 
#administrador_administrador_LDADD =

simulacion_simulacion_SOURCES = $(simulacionSources) $(libSources)
simulacion_simulacion_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/simulacion
#simulacion_simulacion_LDADD = 
