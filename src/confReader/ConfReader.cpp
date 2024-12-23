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
                            if (!base_v.is_string()) {
                                Out::errorMessage("Argument 'base' for node " + name + " is not string");
                            }
                            PdfGen::Obj obj{};
                            obj.base=base_v.as_string();

                            if (params.contains("font")) {
                                auto font_v = params["font"];
                                if (!font_v.is_string()) {
                                    Out::errorMessage("Argument 'font' for node " + name + " is not string");
                                }
                                obj.args["font"] = string(font_v.as_string().c_str());
                            }
                            if (params.contains("font_size")) {
                                auto font_size_v = params["font_size"];
                                if (font_size_v.is_double()) {
                                    obj.args["font_size"] = std::to_string(font_size_v.as_double());
                                } else if (font_size_v.is_int64()) {
                                    obj.args["font_size"] = std::to_string(font_size_v.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'font_size' for node " + name + " is not numeric");
                                }
                            }
                            PdfGen::objs[name]=obj;
                        } else {
                            Out::errorMessage("Can not find 'base' for node " + name);
                        }
                    }
                }
            }
        }
    }
}