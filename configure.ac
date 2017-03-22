# adna
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([adna],[2],[jake@no.com])
AC_CONFIG_SRCDIR([RomanEmpire.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MMAP
AC_CHECK_FUNCS([ftruncate getcwd realpath])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
