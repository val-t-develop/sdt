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

PdfGen::Object::Object(Type _type, int _x, int _y, string _text, string _font,
                       array<float, 3> _color = {0.0f, 0.0f, 0.0f},
                       array<float, 3> _bgcolor = {1.0f, 1.0f, 1.0f}) {
    type = _type;
    x=_x;y=_y;
    text = _text;
    font = _font;
    color = _color;
    bgcolor = _bgcolor;
    bool found = false;
    for (PdfFont *f : fonts) {
        if (f->GetMetrics().GetFontName() == font) {
            found = true;
            Font=f;
            painter->TextState.SetFont(*f, 18);
            cout << f->GetStringLength(text, painter->TextState);
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

PdfGen::Object::Object(Type _type, int _x, int _y, int _width, int _height, array<float, 3> _color, array<float, 3> _bgcolor) {
    type = _type;
    x=_x;y=_y;
    width=_width;
    height=_height;
    color = _color;
    bgcolor = _bgcolor;
}

PdfGen::PdfGen(vector<shared_ptr<Node>> _nodes) : nodes(_nodes) {}

void PdfGen::genPdf() {
    document = new PdfMemDocument();
    painter = new PdfPainter();
    try {
        auto &page = document->GetPages().CreatePage(
            PdfPage::CreateStandardPageSize(PdfPageSize::A4));
        painter->SetCanvas(page);

        for (shared_ptr<Node> node : nodes) {
            constructObjForNode(node);
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
        objs.push_back(make_shared<Object>(Object::Type::TEXT, 0, 0, node->text, "Arial"));
    } else {
        for (auto el : node->nodes) {
            constructObjForNode(el);
        }
    }
}
