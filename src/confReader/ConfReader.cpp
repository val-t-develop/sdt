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
                        PdfGen::Obj obj{};
                        for (auto param : params) {
                            string pname = param.key();
                            auto pval = param.value();
                            if (pname == "base") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'base' for node " + name + " is not string");
                                }
                                obj.base = pval.as_string();
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
                                if (name!="doc") {
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
                                if (name!="doc") {
                                    Out::errorMessage("Argument 'doc_margin_y' can be applied only for 'doc' node");
                                }
                                if (pval.is_double()) {
                                    obj.args["doc_margin_y"] = std::to_string(pval.as_double());
                                } else if (pval.is_int64()) {
                                    obj.args["doc_margin_y"] = std::to_string(pval.as_int64());
                                } else {
                                    Out::errorMessage("Argument 'doc_margin_y' is not numeric");
                                }
                            } else if (pname == "src") {
                                if (!pval.is_string()) {
                                    Out::errorMessage("Argument 'src' for node " + name + " is not string");
                                }
                                obj.args["src"] = string(pval.as_string().c_str());
                            }// TODO color, bgcolor
                            else {
                                Out::errorMessage("WARNING: argument '" + name + "' is not known and will be ignored");
                            }
                        }
                        PdfGen::objs[name]=obj;
                    }
                }
            }
        }
    }
}