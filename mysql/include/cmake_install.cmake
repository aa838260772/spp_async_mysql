# Install script for directory: /home/syswj/Documents/mysql-connector-c-6.1.11-src/include

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysql.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysql_com.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_command.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysql_time.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_list.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_alloc.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/typelib.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/binary_log_types.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_dbug.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/m_string.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_sys.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_xml.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysql_embed.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_thread.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_thread_local.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/decimal.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/errmsg.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_global.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_getopt.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/sslopt-longopts.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_dir.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/sslopt-vars.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/sslopt-case.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/sql_common.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/keycache.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/m_ctype.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_compiler.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysql_com_server.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_byteorder.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/byte_order_generic.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/byte_order_generic_x86.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/little_endian.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/big_endian.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/thr_cond.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/thr_mutex.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/thr_rwlock.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysql_version.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/my_config.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysqld_ername.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysqld_error.h"
    "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/sql_state.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysql" TYPE DIRECTORY FILES "/home/syswj/Documents/mysql-connector-c-6.1.11-src/include/mysql/" REGEX "/[^/]*\\.h$" REGEX "/psi\\_abi[^/]*$" EXCLUDE)
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

