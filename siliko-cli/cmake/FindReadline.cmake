# FindReadline.cmake: Find Readline
# Copyright 2020 Vincent Damewood
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

message(STATUS "Finding Readline")

find_library(Readline_LIBRARY readline)
if(Readline_LIBRARY)
	message(STATUS "  Readline library found at ${Readline_LIBRARY}")
else()
	message(STATUS "  Readline library not found.")
endif()

find_path(Readline_INCLUDE_DIR NAMES readline/readline.h)
if(Readline_INCLUDE_DIR)
	message(STATUS "  Readline includes found at ${Readline_INCLUDE_DIR}")
else()
	message(STATUS "  Readline includes not found.")
endif()

if(Readline_LIBRARY AND Readline_INCLUDE_DIR)
	add_library(Readline::Readline UNKNOWN IMPORTED GLOBAL)
	set_property(TARGET Readline::Readline PROPERTY IMPORTED_LOCATION "${Readline_LIBRARY}")
	target_include_directories(Readline::Readline INTERFACE "${Readline_INCLUDE_DIR}")
	set(Readline_FOUND 1)
else()
	set(Readline_FOUND 0)
endif()
