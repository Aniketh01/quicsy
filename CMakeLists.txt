cmake_minimum_required(VERSION 3.14)

# openssl (for examples)
set(HAVE_OPENSSL    ${OPENSSL_FOUND})
if(OPENSSL_FOUND)
  set(OPENSSL_INCLUDE_DIRS  ${OPENSSL_INCLUDE_DIR})
else()
  set(OPENSSL_INCLUDE_DIRS  "")
  set(OPENSSL_LIBRARIES     "")
endif()
# libev (for examples)
set(HAVE_LIBEV      ${LIBEV_FOUND})
# libnghttp3 (for examples)
set(HAVE_LIBNGHTTP3 ${LIBNGHTTP3_FOUND})

set(CFLAGS_LIST
    "-std=c11 "
    "-ggdb3 "
    "-Wall "
    "-Wmissing-prototypes -Wmissing-declarations -Wredundant-decls "
    "-Wunused-function -Wunused-value -Wunused-variable "
    "-fstrict-aliasing ")
string(REPLACE "" "" CFLAGS ${CFLAGS_LIST})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ${CFLAGS}")

add_definitions(-DHAVE_CONFIG_H)
# autotools-compatible names
# Sphinx expects relative paths in the .rst files. Use the fact that the files
# below are all one directory level deep.
file(RELATIVE_PATH top_srcdir   "${CMAKE_CURRENT_BINARY_DIR}/dir" "${CMAKE_CURRENT_SOURCE_DIR}")
file(RELATIVE_PATH top_builddir "${CMAKE_CURRENT_BINARY_DIR}/dir" "${CMAKE_CURRENT_BINARY_DIR}")
set(abs_top_srcdir  "${CMAKE_CURRENT_SOURCE_DIR}")
set(abs_top_builddir "${CMAKE_CURRENT_BINARY_DIR}")
# libngtcp2.pc (pkg-config file)
set(prefix          "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix     "${CMAKE_INSTALL_PREFIX}")
set(libdir          "${CMAKE_INSTALL_FULL_LIBDIR}")
set(includedir      "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
set(VERSION         "${PACKAGE_VERSION}")
# For init scripts and systemd service file (in contrib/)
set(bindir          "${CMAKE_INSTALL_FULL_BINDIR}")
set(sbindir         "${CMAKE_INSTALL_FULL_SBINDIR}")
foreach(name
		../ngtcp2/lib/libngtcp2.pc
		../ngtcp2/lib/includes/ngtcp2/version.h
		)
	configure_file("${name}.in" "${name}" @ONLY)
endforeach()


include_directories(
	${CMAKE_SOURCE_DIR}../ngtcp2/lib/includes
	${CMAKE_BINARY_DIR}../ngtcp2/lib/includes
	${CMAKE_SOURCE_DIR}../ngtcp2/crypto/includes

	${OPENSSL_INCLUDE_DIRS}
	${LIBEV_INCLUDE_DIRS}
	${LIBNGHTTP3_INCLUDE_DIRS}
	)

link_libraries(
	ngtcp2_crypto_openssl
	ngtcp2
	${OPENSSL_LIBRARIES}
	${LIBEV_LIBRARIES}
	${LIBNGHTTP3_LIBRARIES}
	)

set(client_SOURCES
	quic_client.cc
	)

#set(server_SOURCES
#	)

set(quicsy_SOURCES
	quicsy.cc
	quicsy_tls.cc
	)

add_executable(quicsy ${quicsy_SOURCES})

set_target_properties(quicsy PROPERTIES
	COMPILE_FLAGS "${WARNCXXFLAGS}"
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	)
