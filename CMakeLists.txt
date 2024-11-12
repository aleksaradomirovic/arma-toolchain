# This is a collection of Arma 3 modding tools.
# Copyright (C) 2024  Aleksa Radomirovic
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, 51 Franklin Street,
# Fifth Floor, Boston, MA 02110-1301, USA.

cmake_minimum_required(VERSION 3.25)
project(decentchat LANGUAGES C)

add_compile_options(
    $<$<COMPILE_LANGUAGE:C>:-std=gnu17>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>
    -O2 -g -fanalyzer
    -Wall -Wextra
)

add_subdirectory(cfs)
set_property(
    DIRECTORY cfs
    PROPERTY EXCLUDE_FROM_ALL TRUE
)

add_executable(
    pbo
    pbo/list.c
    pbo/main.c
    pbo/pbo.c
)
target_link_libraries(
    pbo PRIVATE cfs-static
)
