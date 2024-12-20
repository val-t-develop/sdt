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
        PdfFont * Font;
        vector<int> text_splits = vector<int>();
    public:
        enum class Type {
            TEXT, RECT, IMG, VID
        };
        Type type;
        array<double,2> coord;
        array<double,2> size;
        string text;
        array<double,3> color;
        array<double,3> bgcolor;
        string font;
        double font_size;
        Object() = default;
        Object(Type _type, array<double,2> _coord, array<double,2> _size, string _text, array<double,3> _color, array<double,3> _bgcolor, string _font, double _font_size); // __basic
        Object(Type _type, array<double,2> _coord, string _text, string _font, double _font_size, array<double,3> _color, array<double,3> _bgcolor); // TEXT
        Object(Type _type, array<double,2> _coord, array<double,2> _size, array<double,3> _color, array<double,3> _bgcolor); // RECT
        void render();
    };
    static map<string, Object> objTypes;
    static void setBasicObjTypes();

    vector<shared_ptr<Node>> nodes;
    vector<shared_ptr<Object>> objs;

    array<double,2> prev_coord;
    array<double,2> prev_size;

    PdfGen(vector<shared_ptr<Node>> _nodes);
    void genPdf();
    array<double,3> constructObjForNode(shared_ptr<Node> node, array<double,3> coord);
    static array<double,2> convertCoord(array<double,2> coord);
};