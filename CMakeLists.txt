#This file is part of MATIOCPP
#
#MATIOCPP is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#MATIOCPP is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with MATIOCPP.  If not, see <http://www.gnu.org/licenses/>.
project(MATIOCPP)
cmake_minimum_required(VERSION 2.8)

#####################################################################
# Find necessary libraries
set(MATIOCPP_LIBRARIES "" CACHE INTERNAL "MATIOCPP LIBRARIES")

#####################################################################
# Setup name
IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	SET(PACKAGE_NAME "matiocpp-dbg")
	SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ${CMAKE_CXX_DEBUG_FLAGS} -DTIXML_USE_TICPP=YES")
ELSE( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	SET(PACKAGE_NAME "matiocpp")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIXML_USE_TICPP=YES")
ENDIF()

#####################################################################
# Setup packaging
set(CPACK_PACKAGE_DESCRIPTION "matiocpp library")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "matiocpp wrapper written in C++")
set(CPACK_PACKAGE_NAME ${PACKAGE_NAME})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_PACKAGE_CONTACT "Salva Ardid. Based on previous version by Lars Johannesen")
set(CPACK_PACKAGE_VENDOR "Salva Ardid. Based on previous version by Lars Johannesen")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "4-ubuntu1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

if(UNIX)
	set(CPACK_GENERATOR "DEB;TGZ")
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CMAKE_SYSTEM_PROCESSOR}")
elseif(WIN32)
	set(CPACK_GENERATOR "TGZ")
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
endif()

include(CPack)

#####################################################################
# Generate config.hpp
if(NOT WITH_SHARED)
	if(UNIX)
		set(WITH_SHARED 1)
	else()
		set(WITH_SHARED 0)
	endif()
endif()
message(STATUS "Shared: ${WITH_SHARED}")

if(NOT WITH_STATIC)
	if(WIN32)
		set(WITH_STATIC 1)
	else()
		set(WITH_STATIC 0)
	endif()
endif()
message(STATUS "Static: ${WITH_STATIC}")

if(NOT WITH_STATIC AND NOT WITH_SHARED)
	message(FATAL_ERROR "Must built with either static or shared!")
endif()

add_subdirectory(src)
