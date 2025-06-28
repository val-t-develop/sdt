/*  SDT - Scientific Document Tool
 *  Copyright (C) 2022-2025  Valentyn Tymchyshyn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develop@gmail.com)
 *
 *  Some definitions used in project.
 */
#pragma once

#include <array>
#include <cstdint>
#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using std::array;
using std::cerr;
using std::cout;
using std::endl;
using std::make_shared;
using std::make_unique;
using std::pair;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;
namespace fs = std::filesystem;
using std::exception;
using std::fstream;
using std::ifstream;
using std::map;
using std::max;
using std::min;
using std::ofstream;
using std::stack;

vector<string> split(string s, string delimiter);
int64_t power(int base, int exponent);