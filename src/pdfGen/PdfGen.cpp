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
#include "PdfGen.hpp"
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>

vector<PdfFont*> PdfGen::fonts;
PdfMemDocument* PdfGen::document;
PdfPainter* PdfGen::painter;
PdfPage* PdfGen::page;

PdfGen::Object::Object(Type _type, array<double,2> _coord, string _text, string _font,
                       array<double, 3> _color = {0.0, 0.0, 0.0},
                       array<double, 3> _bgcolor = {1.0, 1.0, 1.0}) {
    type = _type;
    coord=_coord;
    text = _text;
    font = _font;
    color = _color;
    bgcolor = _bgcolor;
    bool found = false;
    for (PdfFont *f : fonts) {
        if (f->GetMetrics().GetFontName() == font) {
            found = true;
            Font=f;
            break;
        }
    }
    if (!found) {
        PdfFont *f = document->GetFonts().SearchFont("Arial");
        if (f == nullptr) {
            Out::errorMessage("Can not find font " + font);
            exit(1);
        }
        fonts.push_back(f);
        Font=f;
    }
}

PdfGen::Object::Object(Type _type, array<double,2> _coord, array<double,2> _size,
                       array<double, 3> _color, array<double, 3> _bgcolor) {
    type = _type;
    coord=_coord;
    size = _size;
    color = _color;
    bgcolor = _bgcolor;
}

void PdfGen::Object::render() {
    if (type == Type::TEXT) {
        painter->TextState.SetFont(*Font, 18);
        painter->GraphicsState.SetFillColor(PdfColor(color[0], color[1], color[2]));
        painter->DrawText(text, coord[0], page->GetRect().Height-coord[1]-18);
    } else if (type == Type::RECT) {
        painter->GraphicsState.SetFillColor(PdfColor(bgcolor[0], bgcolor[1], bgcolor[2]));
        painter->GraphicsState.SetStrokeColor(PdfColor(color[0], color[1], color[2]));
        painter->DrawRectangle(coord[0], coord[1], size[0], size[1], PdfPathDrawMode::Fill);
    }
}

PdfGen::PdfGen(vector<shared_ptr<Node>> _nodes) : nodes(_nodes) {}

void PdfGen::genPdf() {
    document = new PdfMemDocument();
    painter = new PdfPainter();
    try {
        page = &(document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)));
        painter->SetCanvas(*page);

        for (shared_ptr<Node> node : nodes) {
            constructObjForNode(node);
        }

        for (shared_ptr<Object> obj : objs) {
            obj->render();
        }


        painter->FinishDrawing();

        document->GetMetadata().SetCreator(
            PdfString("SDT - Scientific Document Tool (powered by PoDoFo)"));
        document->GetMetadata().SetAuthor(PdfString(""));
        document->GetMetadata().SetTitle(PdfString(""));
        document->GetMetadata().SetSubject(PdfString(""));
        document->GetMetadata().SetKeywords(vector<string>({""}));

        document->Save(ArgsParser::output->getFilename());
    } catch (PdfError &e) {
        try {
            painter->FinishDrawing();
        } catch (...) {}
        throw e;
    }
    delete painter;
    delete document;
}

void PdfGen::constructObjForNode(shared_ptr<Node> node) {
    if (node->name=="text") {
        objs.push_back(make_shared<Object>(Object::Type::TEXT, array<double,2>{0.0, 0.0}, node->text, "Arial"));
    } else if (node->name=="rect") {
        objs.insert(objs.begin(), make_shared<Object>(Object::Type::RECT, array<double,2>{0.0, 0.0}, array<double,2>{100.0, 100.0}, array<double,3>{1.0, 0.0, 0.0}, array<double,3>{1.0, 0.0, 0.0}));
        for (auto el : node->nodes) {
            constructObjForNode(el);
        }
    } else {
        for (auto el : node->nodes) {
            constructObjForNode(el);
        }
    }
}
