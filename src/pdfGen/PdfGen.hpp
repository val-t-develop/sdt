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
#include <ast/Node.hpp>
#include <podofo/podofo.h>

using namespace PoDoFo;

class PdfGen {
public:
    static vector<PdfFont*> fonts;
    static PdfMemDocument* document;
    static PdfPainter* painter;
    static PdfPage* page;
    class Object {
    public:
        enum class Type {
            TEXT, RECT, IMG, VID
        };
        Type type;
        int x, y;
        int width;
        int height;
        string text;
        array<float,3> color;
        array<float,3> bgcolor;
        string font;
        PdfFont* Font;
        Object(Type _type, int _x, int _y, string _text, string _font, array<float,3> _color, array<float,3> _bgcolor); // TEXT
        Object(Type _type, int _x, int _y, int _width, int _height, array<float,3> _color, array<float,3> _bgcolor); // RECT
        void render();
    };

    vector<shared_ptr<Node>> nodes;
    vector<shared_ptr<Object>> objs;
    PdfGen(vector<shared_ptr<Node>> _nodes);
    void genPdf();
    void constructObjForNode(shared_ptr<Node> node);
};