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
include_directories(${MATIOCPP_SOURCE_DIR}/src)
link_directories(${LINK_DIRECTORIES}/src)

IF( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
	install(DIRECTORY matiocpp
		DESTINATION include
		FILES_MATCHING PATTERN "*.hpp"
			       PATTERN "*.h"
		)
endif()
