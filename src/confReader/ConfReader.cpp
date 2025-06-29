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
                            } else if (pname == "font-size") {
                                if (pval.is_double()) {
                                    obj.args["font-size"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["font-size"] = std::to_string(pval.as_int64());
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
                            } else if (pname == "font-color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'font-color' for node " + name + " is not string");
                                }
                                string font_color(pval.as_string().c_str());
                                if (font_color.size() != 6) {
                                    Out::errorMessage("Argument 'font-color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : font_color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'font-color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                    }
                                }
                                obj.args["font-color"] = font_color;
                            } else if (pname == "background-color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'background-color' for node " + name + " is not string");
                                }
                                string background_color(pval.as_string().c_str());
                                if (background_color.size() != 6) {
                                    Out::errorMessage("Argument 'background-color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : background_color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'background-color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                    }
                                }
                                obj.args["background-color"] = background_color;
                            } else if (pname == "left-border-width") {
                                if (pval.is_double()) {
                                    obj.args["left-border-width"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["left-border-width"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'left-border-width' is not numeric");
                                }
                            } else if (pname == "left-border-style") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'left-border-style' for node " + name + " is not string");
                                }
                                obj.args["left-border-style"] = string(pval.as_string().c_str());
                            } else if (pname == "left-border-color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'left-border-color' for node " + name + " is not string");
                                }
                                string left_border_color(pval.as_string().c_str());
                                if (left_border_color.size() != 6) {
                                    Out::errorMessage("Argument 'left-border-color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : left_border_color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'left-border-color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                        }
                                }
                                obj.args["left-border-color"] = left_border_color;
                            } else if (pname == "right-border-width") {
                                if (pval.is_double()) {
                                    obj.args["right-border-width"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["right-border-width"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'right-border-width' is not numeric");
                                }
                            } else if (pname == "right-border-style") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'right-border-style' for node " + name + " is not string");
                                }
                                obj.args["right-border-style"] = string(pval.as_string().c_str());
                            } else if (pname == "right-border-color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'right-border-color' for node " + name + " is not string");
                                }
                                string right_border_color(pval.as_string().c_str());
                                if (right_border_color.size() != 6) {
                                    Out::errorMessage("Argument 'right-border-color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : right_border_color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'right-border-color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                        }
                                }
                                obj.args["right-border-color"] = right_border_color;
                            } else if (pname == "top-border-width") {
                                if (pval.is_double()) {
                                    obj.args["top-border-width"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["top-border-width"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'top-border-width' is not numeric");
                                }
                            } else if (pname == "top-border-style") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'top-border-style' for node " + name + " is not string");
                                }
                                obj.args["top-border-style"] = string(pval.as_string().c_str());
                            } else if (pname == "top-border-color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'top-border-color' for node " + name + " is not string");
                                }
                                string top_border_color(pval.as_string().c_str());
                                if (top_border_color.size() != 6) {
                                    Out::errorMessage("Argument 'top-border-color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : top_border_color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'top-border-color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                        }
                                }
                                obj.args["top-border-color"] = top_border_color;
                            } else if (pname == "bottom-border-width") {
                                if (pval.is_double()) {
                                    obj.args["bottom-border-width"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["bottom-border-width"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'bottom-border-width' is not numeric");
                                }
                            } else if (pname == "bottom-border-style") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'bottom-border-style' for node " + name + " is not string");
                                }
                                obj.args["bottom-border-style"] = string(pval.as_string().c_str());
                            } else if (pname == "bottom-border-color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'bottom-border-color' for node " + name + " is not string");
                                }
                                string bottom_border_color(pval.as_string().c_str());
                                if (bottom_border_color.size() != 6) {
                                    Out::errorMessage("Argument 'bottom-border-color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : bottom_border_color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'bottom-border-color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                        }
                                }
                                obj.args["bottom-border-color"] = bottom_border_color;
                            } else if (pname == "border-width") {
                                if (pval.is_double()) {
                                    string border_width = std::to_string(pval.as_double());
                                    obj.args["left-border-width"] = !obj.args.contains("left-border-width") ? border_width : obj.args["left-border-width"];
                                    obj.args["right-border-width"] = !obj.args.contains("right-border-width") ? border_width : obj.args["right-border-width"];
                                    obj.args["top-border-width"] = !obj.args.contains("top-border-width") ? border_width : obj.args["top-border-width"];
                                    obj.args["bottom-border-width"] = !obj.args.contains("bottom-border-width") ? border_width : obj.args["bottom-border-width"];
                                } else if (pval.is_int64()) {
                                    string border_width = std::to_string(pval.as_int64());
                                    obj.args["left-border-width"] = !obj.args.contains("left-border-width") ? border_width : obj.args["left-border-width"];
                                    obj.args["right-border-width"] = !obj.args.contains("right-border-width") ? border_width : obj.args["right-border-width"];
                                    obj.args["top-border-width"] = !obj.args.contains("top-border-width") ? border_width : obj.args["top-border-width"];
                                    obj.args["bottom-border-width"] = !obj.args.contains("bottom-border-width") ? border_width : obj.args["bottom-border-width"];
                                } else {
                                    Out::errorMessage("Argument 'border-width' is not numeric");
                                }
                            } else if (pname == "border-style") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'border-style' for node " + name + " is not string");
                                }
                                string border_style = string(pval.as_string().c_str());
                                obj.args["left-border-style"] = !obj.args.contains("left-border-style") ? border_style : obj.args["left-border-style"];
                                obj.args["right-border-style"] = !obj.args.contains("right-border-style") ? border_style : obj.args["right-border-style"];
                                obj.args["top-border-style"] = !obj.args.contains("top-border-style") ? border_style : obj.args["top-border-style"];
                                obj.args["bottom-border-style"] = !obj.args.contains("bottom-border-style") ? border_style : obj.args["bottom-border-style"];
                            } else if (pname == "border-color") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'border-color' for node " + name + " is not string");
                                }
                                string border_color(pval.as_string().c_str());
                                if (border_color.size() != 6) {
                                    Out::errorMessage("Argument 'border-color' for node " + name +
                                                      " is not 6 character string");
                                }
                                for (auto c : border_color) {
                                    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                                        c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                                        c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                                        Out::errorMessage("Argument 'border-color' for node " + name + " can not contain '" +
                                                          c + "' character");
                                        }
                                }
                                obj.args["left-border-color"] = !obj.args.contains("left-border-color") ? border_color : obj.args["left-border-color"];
                                obj.args["right-border-color"] = !obj.args.contains("right-border-color") ? border_color : obj.args["right-border-color"];
                                obj.args["top-border-color"] = !obj.args.contains("top-border-color") ? border_color : obj.args["top-border-color"];
                                obj.args["bottom-border-color"] = !obj.args.contains("bottom-border-color") ? border_color : obj.args["bottom-border-color"];
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