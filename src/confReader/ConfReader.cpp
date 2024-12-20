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
                            PdfGen::Object obj = PdfGen::objTypes[string(base_v.as_string().c_str())];
                            if (params.contains("color")) {
                                auto color_v = params["color"];
                                if (!color_v.is_array()) {
                                    Out::errorMessage("Argument 'color' for node " + name + " is not array");
                                }
                                auto color_arr = color_v.as_array();
                                if (color_arr.size() != 3) {
                                    Out::errorMessage("Argument 'color' for node " + name + " is not array of 3 elements");
                                }
                                for (auto& el : color_arr) {
                                    if (!el.is_double()) {
                                        Out::errorMessage("Argument 'color' for node " + name + " is not double array");
                                    }
                                }
                                obj.color = array<double,3>{color_arr[0].as_double(), color_arr[1].as_double(), color_arr[2].as_double()};
                            }
                            if (params.contains("bgcolor")) {
                                auto bgcolor_v = params["bgcolor"];
                                if (!bgcolor_v.is_array()) {
                                    Out::errorMessage("Argument 'bgcolor' for node " + name + " is not array");
                                }
                                auto bgcolor_arr = bgcolor_v.as_array();
                                if (bgcolor_arr.size() != 3) {
                                    Out::errorMessage("Argument 'bgcolor' for node " + name + " is not array of 3 elements");
                                }
                                for (auto& el : bgcolor_arr) {
                                    if (!el.is_double()) {
                                        Out::errorMessage("Argument 'bgcolor' for node " + name + " is not double array");
                                    }
                                }
                                obj.bgcolor = array<double,3>{bgcolor_arr[0].as_double(), bgcolor_arr[1].as_double(), bgcolor_arr[2].as_double()};
                            }
                            if (params.contains("font")) {
                                auto font_v = params["font"];
                                if (!font_v.is_string()) {
                                    Out::errorMessage("Argument 'font' for node " + name + " is not string");
                                }
                                obj.font = string(font_v.as_string().c_str());
                            }
                            if (params.contains("font_size")) {
                                auto font_size_v = params["font_size"];
                                if (font_size_v.is_double()) {
                                    obj.font_size = font_size_v.as_double();
                                } else if (font_size_v.is_int64()) {
                                    obj.font_size = double(font_size_v.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'font_size' for node " + name + " is not numeric");
                                }
                            }
                            PdfGen::objTypes[name]=obj;
                        } else {
                            Out::errorMessage("Can not find 'base' for node " + name);
                        }
                    }
                }
            }
        } else if (param.key() == "document") {
            if (param.value().is_object()) {
                auto params = param.value().as_object();
                for (auto param : params) {
                    if (param.key() == "margin_x") {
                        if (param.value().is_double()) {
                            PdfGen::margin_x = param.value().as_double();
                        } else if (param.value().is_int64()) {
                            PdfGen::margin_x = double(param.value().as_int64());
                        } else {
                            Out::errorMessage("Argument 'margin_x' is not numeric");
                        }
                    } else if (param.key() == "margin_y") {
                        if (param.value().is_number()) {
                            PdfGen::margin_y = param.value().as_double();
                        } else {
                            Out::errorMessage("Argument 'margin_y' is not numeric");
                        }
                    } else {
                        Out::errorMessage("Unknown argument '" + string(param.key()) + "'.");
                    }
                }
            }
        }
    }
}