/*  SDT - Scientific Document Tool
 *  Copyright (C) 2022-2024  Valentyn Tymchyshyn
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
 *  Reader of config file.
 */
#include "ConfReader.hpp"

#include <pdfGen/PdfGen.hpp>
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>

ConfReader::ConfReader() {
    boost::json::value tmp = boost::json::parse(ArgsParser::conf->readFile());
    if (tmp.is_object()) {
        conf = tmp.as_object();
    }
}

void ConfReader::parse() {
    for (auto param : conf) {
        if (param.key() == "nodes") {
            if (param.value().is_object()) {
                auto nodes = param.value().as_object();
                for (auto node : nodes) {
                    string name = node.key();
                    if (node.value().is_object()) {
                        auto params = node.value().as_object();
                        if (params.contains("base")) {
                            auto base_v = params["base"];
                            if (base_v.is_string()) {}
                            PdfGen::Object obj = PdfGen::objTypes[string(base_v.as_string().c_str())];
                            obj.bgcolor=array<double,3>{0.5,0.5,0.5};
                            obj.color=array<double,3>{0.5,0.5,0.5};
                            PdfGen::objTypes[name]=obj;
                        } else {
                            Out::errorMessage("Can not find 'base' for node " + name);
                        }
                    }
                }
            }
        }
    }
}