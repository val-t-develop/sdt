#  SDT - Scientific Document Tool
#  Copyright (C) 2022-2025  Valentyn Tymchyshyn
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
#
#  Meson.build file for SDT.
#  USE Makefile to compile the project.

project('sdt', 'cpp', default_options : 'cpp_std=c++20')
add_project_arguments('-W', '-Wall', '-Wextra', '-Wpedantic', '-g', language : 'cpp')

boost_dep = dependency('boost', modules : ['json'])
libxml2_dep = dependency('libxml2')

incdir = include_directories('src')
src_files = ['src/main.hpp', 'src/main.cpp']

executable('sdt', src_files, dependencies : [boost_dep, libxml2_dep], include_directories : incdir)
