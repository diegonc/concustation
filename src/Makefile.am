AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = -O0 -g -Wall -Wextra -Wconversion
AM_CPPFLAGS = -I$(srcdir)/lib

libSources = \
	lib/system/System.cc \
	lib/system/System.h \
	lib/utils/NonCopyable.h

productorAutosSources = \
	productorAutos/main.cc

bin_PROGRAMS = \
	productorAutos/productorAutos

productorAutos_productorAutos_SOURCES = $(productorAutosSources) $(libSources)
productorAutos_produtorAutos_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/productorAutos
#produtorAutos_produtorAutos_LDADD = 
