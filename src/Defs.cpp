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
#include "Defs.hpp"

vector<string> split(const string& s, const string& delimiter) {
    size_t pos_start = 0, pos_end;
    const size_t delim_len = delimiter.length();
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        string token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
int64_t power(const int base, const int exponent) {
    int64_t result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}