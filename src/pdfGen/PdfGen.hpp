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
#include <pdfGen/Pages.hpp>
using namespace PoDoFo;

class PdfGen {
  public:
    class Obj {
      public:
        string base;
        map<string, string> args;
    };
    static map<string, Obj> objs;

    xmlDoc *xml_document;

    vector<PdfFont *> fonts;
    PdfMemDocument *document;
    PdfPainter *painter;
    Pages pages;

    PdfGen();
    ~PdfGen();
    void gen();

    /**
     *  coord - {coordinates origin, coordinates that you should try to use first, coordinates, max_left, max_right}
     *
     *  returns {coordinates that have been used, size, coordinates that you should try to use first when rendering next
     * object}
     */
    array<double, 6> genNode(xmlNode *node, map<string, string> &args, array<double, 8> pos, int render);
    void drawRect(map<string, string> &args, double x, double y, double w, double h, string color, string mode, int render);
    void drawText(map<string, string> &args, string text,  double x, double y, int render);
    static void genAttr(xmlAttr *attr, map<string, string> &args);
    static array<double, 3> genColor(const string &str);
};
