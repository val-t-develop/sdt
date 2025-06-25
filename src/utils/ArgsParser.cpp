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
 *  Parser of arguments.
 */
#include "ArgsParser.hpp"
#include <utils/License.hpp>
#include <utils/Out.hpp>

shared_ptr<Path> ArgsParser::output = nullptr;
shared_ptr<Path> ArgsParser::conf = nullptr;
shared_ptr<Path> ArgsParser::src = nullptr;

void ArgsParser::parseArgs(int argc, char **argv) {
    string home = getenv("HOME");
    if (home.size() != 1 && (home.ends_with('/') || home.ends_with('\\'))) {
        home.pop_back();
    }
    vector<string> vec;
    for (int i = 0; i < argc; ++i) {
        auto curr = split(string(argv[i]), " ");
        for (auto el : curr) {
            if (el.find("~") != string::npos) {
                el.replace(el.find("~"), string("~").length(), home);
            }
            vec.push_back(el);
        }
    }
    parseArgs(vec);
}

void ArgsParser::parseArgs(vector<string> args) {
    for (size_t i = 1; i < args.size(); i++) {
        string arg = args[i];
        if (arg == "-o") {
            i++;
            arg = args[i];
            output = make_shared<Path>(arg);
        } else if (arg == "-c") {
            i++;
            arg = args[i];
            conf = make_shared<Path>(arg);
        } else if (arg == "show") {
            i++;
            arg = args[i];
            if (arg == "c" || arg == "w") {
                Out::printMessage(LICENSE);
            }
        } else {
            src = make_shared<Path>(arg);
        }
    }
    if (src == nullptr) {
        Out::printMessage("No input files.");
        exit(0);
    }
}