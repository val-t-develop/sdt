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
 *  Reader of config file.
 */
#include "ConfReader.hpp"

#include <utils/ArgsParser.hpp>
#include <parser/Parser.hpp>
#include <utils/Out.hpp>

ConfReader::ConfReader() {
    boost::json::value tmp = boost::json::parse(ArgsParser::conf->readFile());
    if (tmp.is_object()) {
        conf = tmp.as_object();
    }
}

void ConfReader::parse() {
for (auto arg : conf) {
        if (arg.key() == "nodes") {
            if (arg.value().is_object()) {
                auto nodes = arg.value().as_object();
                for (auto node : nodes) {
                    string name(node.key());
                    if (node.value().is_object()) {
                        auto params = node.value().as_object();
                        Parser::Obj obj{};
                        for (auto param : params) {
                            string pname(param.key());
                            auto pval = param.value();
                            if (pname == "base") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'base' for node " + name + " is not string");
                                }
                                obj.base = string(pval.as_string().c_str());
                            } else if (pname == "font") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'font' for node " + name + " is not string");
                                }
                                obj.args["font"] = string(pval.as_string().c_str());
                            } else if (pname == "font_size") {
                                if (pval.is_double()) {
                                    obj.args["font_size"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["font_size"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'font_size' for node " + name + " is not numeric");
                                }
                            } else if (pname == "doc_margin_x") {
                                if (name != "doc") {
                                    Out::errorMessage("Argument 'doc_margin_x' can be applied only for 'doc' node");
                                }
                                if (pval.is_double()) {
                                    obj.args["doc_margin_x"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["doc_margin_x"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'doc_margin_x' is not numeric");
                                }
                            } else if (pname == "doc_margin_y") {
                                if (name != "doc") {
                                    Out::errorMessage("Argument 'doc_margin_y' can be applied only for 'doc' node");
                                }
                                if (pval.is_double()) {
                                    obj.args["doc_margin_y"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["doc_margin_y"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'doc_margin_y' is not numeric");
                                }
                            } else if (pname == "margin_left") {
                                if (pval.is_double()) {
                                    obj.args["margin_left"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["margin_left"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'margin_left' is not numeric");
                                }
                            } else if (pname == "margin_right") {
                                if (pval.is_double()) {
                                    obj.args["margin_right"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["margin_right"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'margin_right' is not numeric");
                                }
                            } else if (pname == "margin_top") {
                                if (pval.is_double()) {
                                    obj.args["margin_top"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["margin_top"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'margin_top' is not numeric");
                                }
                            } else if (pname == "margin_bottom") {
                                if (pval.is_double()) {
                                    obj.args["margin_bottom"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["margin_bottom"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'margin_bottom' is not numeric");
                                }
                            } else if (pname == "padding_left") {
                                if (pval.is_double()) {
                                    obj.args["padding_left"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["padding_left"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'padding_left' is not numeric");
                                }
                            } else if (pname == "padding_right") {
                                if (pval.is_double()) {
                                    obj.args["padding_right"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["padding_right"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'padding_right' is not numeric");
                                }
                            } else if (pname == "padding_top") {
                                if (pval.is_double()) {
                                    obj.args["padding_top"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["padding_top"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'padding_top' is not numeric");
                                }
                            } else if (pname == "padding_bottom") {
                                if (pval.is_double()) {
                                    obj.args["padding_bottom"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["padding_bottom"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'padding_bottom' is not numeric");
                                }
                            } else if (pname == "margin_x") {
                                if (pval.is_double()) {
                                    obj.args["margin_left"] = std::to_string(pval.as_double());
                                    obj.args["margin_right"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["margin_left"] = std::to_string(pval.as_int64());
                                    obj.args["margin_right"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'margin_x' is not numeric");
                                }
                            } else if (pname == "margin_y") {
                                if (pval.is_double()) {
                                    obj.args["margin_top"] = std::to_string(pval.as_double());
                                    obj.args["margin_bottom"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["margin_top"] = std::to_string(pval.as_int64());
                                    obj.args["margin_bottom"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'margin_y' is not numeric");
                                }
                            } else if (pname == "padding_x") {
                                if (pval.is_double()) {
                                    obj.args["padding_left"] = std::to_string(pval.as_double());
                                    obj.args["padding_right"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["padding_left"] = std::to_string(pval.as_int64());
                                    obj.args["padding_right"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'padding_x' is not numeric");
                                }
                            } else if (pname == "padding_y") {
                                if (pval.is_double()) {
                                    obj.args["padding_top"] = std::to_string(pval.as_double());
                                    obj.args["padding_bottom"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["padding_top"] = std::to_string(pval.as_int64());
                                    obj.args["padding_bottom"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'padding _y' is not numeric");
                                }
                            } else if (pname == "margin") {
                                if (pval.is_double()) {
                                    obj.args["margin_left"] = std::to_string(pval.as_double());
                                    obj.args["margin_right"] = std::to_string(pval.as_double());
                                    obj.args["margin_top"] = std::to_string(pval.as_double());
                                    obj.args["margin_bottom"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["margin_left"] = std::to_string(pval.as_int64());
                                    obj.args["margin_right"] = std::to_string(pval.as_int64());
                                    obj.args["margin_top"] = std::to_string(pval.as_int64());
                                    obj.args["margin_bottom"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'margin' is not numeric");
                                }
                            } else if (pname == "padding") {
                                if (pval.is_double()) {
                                    obj.args["padding_left"] = std::to_string(pval.as_double());
                                    obj.args["padding_right"] = std::to_string(pval.as_double());
                                    obj.args["padding_top"] = std::to_string(pval.as_double());
                                    obj.args["padding_bottom"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["padding_left"] = std::to_string(pval.as_int64());
                                    obj.args["padding_right"] = std::to_string(pval.as_int64());
                                    obj.args["padding_top"] = std::to_string(pval.as_int64());
                                    obj.args["padding_bottom"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'padding' is not numeric");
                                }
                            } else if (pname == "src") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'src' for node " + name + " is not string");
                                }
                                obj.args["src"] = string(pval.as_string().c_str());
                            } else if (pname == "color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'color' for node " + name + " is not string");
                                }
                                string color(pval.as_string().c_str());
                                if (color.size() != 6) {
                                    Out::errorMessage("Argument 'color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                    }
                                }
                                obj.args["color"] = color;
                            } else if (pname == "bgcolor") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'bgcolor' for node " + name + " is not string");
                                }
                                string bgcolor(pval.as_string().c_str());
                                if (bgcolor.size() != 6) {
                                    Out::errorMessage("Argument 'bgcolor' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : bgcolor) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'bgcolor' for node " + name + " can not contain '" +
                                                          c + "' character");
                                    }
                                }
                                obj.args["bgcolor"] = bgcolor;
                            } else {
                                Out::errorMessage("WARNING: argument '" + name + "' is not known and will be ignored");
                            }
                        }
                        Parser::objs[name] = obj;
                    }
                }
            }
        }
    }
}