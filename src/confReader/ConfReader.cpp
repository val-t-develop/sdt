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

#include <parser/Parser.hpp>
#include <utils/ArgsParser.hpp>
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
                            string pval;
                            if (param.value().is_string()) {
                                pval = string(param.value().as_string().c_str());
                            } else if (param.value().is_double()) {
                                pval = to_string(param.value().as_double());
                            } else if (param.value().is_int64()) {
                                pval = to_string(param.value().as_int64());
                            }
                            parseOption(name, pname, pval, obj.args);
                        }
                        Parser::objs[name] = obj;
                    }
                }
            }
        }
    }
}

void ConfReader::parseOption(string node, string name, string val, map<string, string> &attrs) {
    if (name == "font" || name == "left-border-style" || name == "right-border-style" || name == "top-border-style" ||
        name == "bottom-border-style") {
        attrs["font"] = val;
    } else if (name == "font-size" || name == "left-border-width" || name == "right-border-width" ||
               name == "top-border-width" || name == "bottom-border-width" || name == "margin-left" ||
               name == "margin-right" || name == "margin-top" || name == "margin-bottom" || name == "padding-left" ||
               name == "padding-right" || name == "padding-top" || name == "padding-bottom") {
        if (isNum(val)) {
            attrs[name] = val;
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else if (name == "font-color" || name == "background-color" || name == "left-border-color" ||
               name == "right-border-color" || name == "top-border-color" || name == "bottom-border-color") {
        if (isHexColor(val)) {
            attrs[name] = val;
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not valid");
        }
    } else if (name == "border-width") {
        if (isNum(val)) {
            attrs["left-border-width"] = !attrs.contains("left-border-width") ? val : attrs["left-border-width"];
            attrs["right-border-width"] = !attrs.contains("right-border-width") ? val : attrs["right-border-width"];
            attrs["top-border-width"] = !attrs.contains("top-border-width") ? val : attrs["top-border-width"];
            attrs["bottom-border-width"] = !attrs.contains("bottom-border-width") ? val : attrs["bottom-border-width"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else if (name == "border-style") {
        attrs["left-border-style"] = !attrs.contains("left-border-style") ? val : attrs["left-border-style"];
        attrs["right-border-style"] = !attrs.contains("right-border-style") ? val : attrs["right-border-style"];
        attrs["top-border-style"] = !attrs.contains("top-border-style") ? val : attrs["top-border-style"];
        attrs["bottom-border-style"] = !attrs.contains("bottom-border-style") ? val : attrs["bottom-border-style"];
    } else if (name == "border-color") {
        if (isHexColor(val)) {
            attrs["left-border-color"] = !attrs.contains("left-border-color") ? val : attrs["left-border-color"];
            attrs["right-border-color"] = !attrs.contains("right-border-color") ? val : attrs["right-border-color"];
            attrs["top-border-color"] = !attrs.contains("top-border-color") ? val : attrs["top-border-color"];
            attrs["bottom-border-color"] = !attrs.contains("bottom-border-color") ? val : attrs["bottom-border-color"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not valid");
        }
    } else if (name == "margin-horizontal") {
        if (isNum(val)) {
            attrs["margin-left"] = !attrs.contains("margin-left") ? val : attrs["margin-left"];
            attrs["margin-right"] = !attrs.contains("margin-right") ? val : attrs["margin-right"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else if (name == "margin-vertical") {
        if (isNum(val)) {
            attrs["margin-top"] = !attrs.contains("margin-top") ? val : attrs["margin-top"];
            attrs["margin-bottom"] = !attrs.contains("margin-bottom") ? val : attrs["margin-bottom"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else if (name == "padding-horizontal") {
        if (isNum(val)) {
            attrs["padding-left"] = !attrs.contains("padding-left") ? val : attrs["padding-left"];
            attrs["padding-right"] = !attrs.contains("padding-right") ? val : attrs["padding-right"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else if (name == "padding-vertical") {
        if (isNum(val)) {
            attrs["padding-top"] = !attrs.contains("padding-top") ? val : attrs["padding-top"];
            attrs["padding-bottom"] = !attrs.contains("padding-bottom") ? val : attrs["padding-bottom"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else if (name == "margin") {
        if (isNum(val)) {
            attrs["margin-left"] = !attrs.contains("margin-left") ? val : attrs["margin-left"];
            attrs["margin-right"] = !attrs.contains("margin-right") ? val : attrs["margin-right"];
            attrs["margin-top"] = !attrs.contains("margin-top") ? val : attrs["margin-top"];
            attrs["margin-bottom"] = !attrs.contains("margin-bottom") ? val : attrs["margin-bottom"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else if (name == "padding") {
        if (isNum(val)) {
            attrs["padding-left"] = !attrs.contains("padding-left") ? val : attrs["padding-left"];
            attrs["padding-right"] = !attrs.contains("padding-right") ? val : attrs["padding-right"];
            attrs["padding-top"] = !attrs.contains("padding-top") ? val : attrs["padding-top"];
            attrs["padding-bottom"] = !attrs.contains("padding-bottom") ? val : attrs["padding-bottom"];
        } else {
            Out::errorMessage("Argument '" + name + "' for node " + node + " is not numeric");
        }
    } else {
        Out::errorMessage("WARNING: argument '" + name + "' is not known and will be ignored");
    }
}

bool ConfReader::isHexColor(string str) {
    if (str.size() != 6) {
        Out::errorMessage("Color " + str + " is not 6 character string");
        return false;
    }
    for (auto c : str) {
        if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' &&
            c != '9' && c != 'A' && c != 'B' && c != 'C' && c != 'D' && c != 'E' && c != 'F') {
            Out::errorMessage("Color " + str + " can not contain '" + c + "' character");
            return false;
        }
    }
    return true;
}

bool ConfReader::isNum(string str) {
    try {
        std::stod(str);
    } catch (...) {
        return false;
    }
    return true;
}