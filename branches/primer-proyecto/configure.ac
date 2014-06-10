#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])
AC_INIT([concustation], [1.0.0], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/lib/system/System.cc])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
# override default flags
: ${CXXFLAGS:="-g -O0"}
AC_PROG_CXX
AC_PROG_CC

# Check for pkg-config compatible modules.

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([mkfifo strerror])

AC_CHECK_FUNC([argp_parse], ,
	AC_MSG_ERROR([argp library not found.]))

AC_CHECK_FUNC([sigsuspend], ,
	AC_MSG_ERROR([sigsuspend is required]))

AC_CONFIG_FILES([Makefile
		 src/Makefile])

AC_OUTPUT
