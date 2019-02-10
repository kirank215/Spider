AC_PREREQ(2.60)
AC_INIT([spider],
        [1.0],
        [yaka@assistants.epita.fr],
        [spider],
        [https://assistants.epita.fr/])

AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([
                  foreign
                  -Wall
                  -Werror
                  silent-rules
                  subdir-objects
])

PKG_CHECK_MODULES([GTEST], [gtest, gtest_main])
AC_SUBST([GTEST_CFLAGS])
AC_SUBST([GTEST_LIBS])

AM_PROG_AR
AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])

AX_CHECK_COMPILE_FLAG([-pedantic],
                      [CXXFLAGS="$CXXFLAGS -pedantic"])

AX_COMPILER_FLAGS

LT_INIT([
         disable-shared
         pic-only
 ])

AX_BOOST_BASE

AM_SILENT_RULES([yes])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
