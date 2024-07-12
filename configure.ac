# Prelude.
AC_PREREQ([2.72])
AC_INIT([liblocket], [0.0])
AC_CONFIG_SRCDIR([src/socket.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign tar-ustar -Wall -Werror])
LT_PREREQ([2.5.0.14-9a4a-dirty])
LT_INIT([])
AC_LANG([C++])

# Checks for programs.
AC_PROG_AR
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AM_PROG_AR

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS([netdb.h])
AC_CHECK_HEADERS([netinet/in.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([socket])

# Output files.
AC_CONFIG_FILES([Makefile src/Makefile include/liblocket/Makefile])
AC_CONFIG_SUBDIRS([deps/bits-and-bytes])
AC_OUTPUT
