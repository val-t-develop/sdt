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
#include <podofo/podofo.h>
using namespace PoDoFo;

class PdfGen {
public:
    class Obj {
    public:
        string base;
        map<string,string> args;
    };
    static map<string, Obj> objs;

    xmlDoc *xml_document;

    vector<PdfFont*> fonts;
    PdfMemDocument* document;
    PdfPainter* painter;
    PdfPage* page;

    PdfGen();
    ~PdfGen();
    void gen();

    array<double,3> genNode(xmlNode* node, map<string,string>& args, array<double,3> coord);
    array<double,2> convertCoord(array<double,2> coord, map<string,string>& args);
};
