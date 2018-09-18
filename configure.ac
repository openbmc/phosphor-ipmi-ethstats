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
AX_CXX_COMPILE_STDCXX_14([noext])
AX_APPEND_COMPILE_FLAGS([-Wall -Werror], [CXXFLAGS])

# Checks for libraries.
AC_CHECK_HEADER([host-ipmid], [AC_MSG_ERROR(["phosphor-host-ipmid required and not found."])])
AC_CHECK_HEADER(experimental/any, ,[AC_MSG_ERROR([Could not find experimental/any...libstdc++fs developement package required])])

# Checks for library functions.
LT_INIT # Required for systemd linking

# Do you want to install with the Google OEM Number as well.
AC_ARG_ENABLE([google],
    AS_HELP_STRING([--enable-google], [Enable registering with Google OEN])
)
AC_ARG_VAR(ENABLE_GOOGLE, [Enable registering with Google OEN])
AS_IF([test "x$enable_google" == "xyes"],
      [ENABLE_GOOGLE="yes"]
      AC_DEFINE_UNQUOTED([ENABLE_GOOGLE], ["$ENABLE_GOOGLE"], [Enable registering with Google OEN])
)

# Create configured output
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
