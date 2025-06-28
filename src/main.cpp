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
 *  Main project file.
 */

#include <main.hpp>
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>
#include <confReader/ConfReader.hpp>
#include <parser/Parser.hpp>

int main(int argc, char **argv) {
    Main::main(argc, argv);
    return 0;
}

void Main::main(int argc, char **argv) {
    ArgsParser::parseArgs(argc, argv);
    if (ArgsParser::src->isFile()) {
        if (ArgsParser::conf != nullptr) {
            ConfReader conf_reader;
            conf_reader.parse();
        }
        Parser parser{};
        parser.parse();
    } else {
        Out::errorMessage("Processing directories is unsupported");
    }

    string command = "fop -xml '"+ArgsParser::src->getName()+".xml' -xsl '"+ArgsParser::src->getName()+".xsl' -pdf '"+ArgsParser::output->getName()+"'";
    system(command.c_str());
    if (!ArgsParser::debug) {
        string rm_command = "rm '"+ArgsParser::src->getName()+".xml' '"+ArgsParser::src->getName()+".xsl'";
        system(rm_command.c_str());
    }
}