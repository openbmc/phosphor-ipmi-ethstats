# Initialization
AC_PREREQ([2.69])
AC_INIT([phosphor-ipmi-ethstats], [1.0], [https://www.github.com/openbmc/phosphor-ipmi-ethstats/issues])
AC_LANG([C++])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign dist-xz])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX_17([noext])
AX_APPEND_COMPILE_FLAGS([-Wall -Werror], [CXXFLAGS])

# Checks for libraries.
PKG_CHECK_MODULES([LIBIPMID], [libipmid])
AC_CHECK_HEADER(experimental/filesystem, ,[AC_MSG_ERROR([Could not find experimental/filesystem...libstdc++fs developement package required])])

# Checks for library functions.
LT_INIT # Required for systemd linking

# Do you want to install with the Google OEM Number as well.
AC_ARG_ENABLE([google],
    AS_HELP_STRING(
        [--enable-google], [Enable registering with Google OEN.]
    )
)
AM_CONDITIONAL(ENABLE_GOOGLE, [test "x$enable_google" = "xyes"])
AS_IF([test "x$enable_google" = "xyes"],
    [AC_DEFINE(ENABLE_GOOGLE, [1], [Register with Google OEN.])],
    [AC_DEFINE(ENABLE_GOOGLE, [0], [Do not register with Google OEN.])]
)

# Create configured output
AC_CONFIG_FILES([Makefile test/Makefile])
AC_OUTPUT
