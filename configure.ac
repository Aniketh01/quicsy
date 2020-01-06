AC_INIT([quicsy], [0.1], [anikethgireesh@gmail.com])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_CXX([g++])
LT_INIT([dlopen])
LTDL_INIT([recursive])
LT_CONFIG_LTDL_DIR([libltdl])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIRS([m4])

AC_SUBST(LT_CURRENT, 0)
AC_SUBST(LT_REVISION, 0)
AC_SUBST(LT_AGE, 0)

AC_SUBST(CRYPTO_OPENSSL_LT_CURRENT, 0)
AC_SUBST(CRYPTO_OPENSSL_LT_REVISION, 0)
AC_SUBST(CRYPTO_OPENSSL_LT_AGE, 0)

# openssl (for examples)
PKG_CHECK_MODULES([OPENSSL], [openssl >= 1.1.1],
                  [have_openssl=yes], [have_openssl=no])
if test "x${have_openssl}" = "xno"; then
  AC_MSG_NOTICE($OPENSSL_PKG_ERRORS)
fi
AM_CONDITIONAL([HAVE_OPENSSL], [ test "x${have_openssl}" = "xyes" ])

# libnghttp3 (for examples)
PKG_CHECK_MODULES([LIBNGHTTP3], [libnghttp3 >= 0.0.0],
                  [have_libnghttp3=yes], [have_libnghttp3=no])
if test "${have_libnghttp3}" = "xno"; then
  AC_MSG_NOTICE($LIBNGHTTP3_PKG_ERRORS)
fi
AM_CONDITIONAL([HAVE_NGHTTP3], [ test "x${have_libnghttp3}" = "xyes" ])

# libev (for examples)
# libev does not have pkg-config file.  Check it in an old way.
save_LIBS=$LIBS
# android requires -lm for floor
AC_CHECK_LIB([ev], [ev_time], [have_libev=yes], [have_libev=no], [-lm])
if test "x${have_libev}" = "xyes"; then
  AC_CHECK_HEADER([ev.h], [have_libev=yes], [have_libev=no])
  if test "x${have_libev}" = "xyes"; then
    LIBEV_LIBS=-lev
    LIBEV_CFLAGS=
    AC_SUBST([LIBEV_LIBS])
    AC_SUBST([LIBEV_CFLAGS])
  fi
fi
LIBS=$save_LIBS

AC_OUTPUT

AC_MSG_NOTICE([summary of build options:

			   Package version: ${VERSION}
			   Library version: $LT_CURRENT:$LT_REVISION:$LT_AGE
			   Install prefix:  ${prefix}
			   System types:
			   Build:         ${build}
			   Host:          ${host}
			   Target:        ${target}
			   Compiler:
			   C preprocessor: ${CPP}
			   CPPFLAGS:       ${CPPFLAGS}
			   C compiler:     ${CC}
			   CFLAGS:         ${CFLAGS}
			   C++ compiler:   ${CXX}
			   CXXFLAGS:       ${CXXFLAGS}
			   LDFLAGS:        ${LDFLAGS}
			   WARNCFLAGS:     ${WARNCFLAGS}
			   WARNCXXFLAGS:   ${WARNCXXFLAGS}
			   EXTRACFLAG:     ${EXTRACFLAG}
			   LIBS:           ${LIBS}
			   Library:
			   Shared:         ${enable_shared}
			   Static:         ${enable_static}
			   Crypto helper libraries:
			   libngtcp2_crypto_openssl: ${have_openssl}
			   Test:
			   CUnit:          ${have_cunit} (CFLAGS='${CUNIT_CFLAGS}' LIBS='${CUNIT_LIBS}')
			   Debug:
			   Debug:          ${debug} (CFLAGS='${DEBUGCFLAGS}')
			   Libs:
			   OpenSSL:        ${have_openssl} (CFLAGS='${OPENSSL_CFLAGS}' LIBS='${OPENSSL_LIBS}')
			   Libev:          ${have_libev} (CFLAGS='${LIBEV_CFLAGS}' LIBS='${LIBEV_LIBS}')
			   Libngtcp2:	   ${have_libngtcp2} (CFLAGS='${LIBNGTCP@_CFLAGS}' LIBS='${LIBNGTCP2_LIBS}')
			   Libnghttp3:     ${have_libnghttp3} (CFLAGS='${LIBNGHTTP3_CFLAGS}' LIBS='${LIBNGHTTP3_LIBS}')
			   ])
