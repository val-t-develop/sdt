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
 *  Generator of pdf.
 */
#pragma once
#include <Defs.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>

class PdfGen {
public:
    class Obj {
    public:
        map<string,double> num_args;
        map<string,string> str_args;
        map<string,array<double,2>> num2_args;
        map<string,array<double,3>> num3_args;
    };
    static vector<Obj> objs;
    xmlDoc *document;
    PdfGen();
    ~PdfGen();
    void gen();
};
