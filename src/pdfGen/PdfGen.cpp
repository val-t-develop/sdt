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

map<string, PdfGen::Object> PdfGen::objTypes = map<string, Object>();
vector<PdfFont*> PdfGen::fonts;
PdfMemDocument* PdfGen::document;
PdfPainter* PdfGen::painter;
PdfPage* PdfGen::page;
double PdfGen::margin_x = 50, PdfGen::margin_y = 50;

void PdfGen::setBasicObjTypes() {
    objTypes["text"] = PdfGen::Object(Object::Type::TEXT, array<double,2>{0.0,0.0}, array<double,2>{0.0,0.0}, "", array<double,3>{0.0,0.0,0.0}, array<double,3>{1.0,1.0,1.0}, "Arial", 18.0);
    objTypes["rect"] = PdfGen::Object(Object::Type::RECT, array<double,2>{0.0,0.0}, array<double,2>{0.0,0.0}, "", array<double,3>{1.0,0.0,0.0}, array<double,3>{1.0,0.0,0.0}, "", 18.0);
}

PdfGen::Object::Object(Type _type, array<double, 2> _coord,
                       array<double, 2> _size, string _text,
                       array<double, 3> _color, array<double, 3> _bgcolor,
                       string _font, double _font_size) : type(_type), coord(_coord), size(_size), text(_text), color(_color), bgcolor(_bgcolor), font(_font), font_size(_font_size), Font(nullptr) {}

PdfGen::Object::Object(Type _type, array<double,2> _coord, string _text, string _font, double _font_size,
                       array<double, 3> _color,
                       array<double, 3> _bgcolor) {
    type = _type;
    coord=_coord;
    text = _text;
    font = _font;
    font_size = _font_size;
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
    painter->TextState.SetFont(*Font, font_size);
    int last_space = 0, last_break = 0;
    for (int i=0; i<text.size(); i++) {
        if (text[i]==' ') {
            string substr = text.substr(last_break, i-last_break+1);
            double l = Font->GetStringLength(substr, painter->TextState);
            if (last_space == 0) {
                if (coord[0]+l>page->GetRect().Width-margin_x*2) {
                    last_break = last_space;
                    text_splits.push_back(last_break);
                }
            } else {
                if (l>page->GetRect().Width-margin_x*2) {
                    last_break = last_space;
                    text_splits.push_back(last_break);
                }
            }
            last_space = i;
        }
    }

    if (text_splits.empty()) {
        size[0] = Font->GetStringLength(text, painter->TextState);
    } else {
        size[0] = Font->GetStringLength(text.substr(text_splits[text_splits.size()-1], text.size()), painter->TextState)-coord[0];
    }
    size[1] = Font->GetLineSpacing(painter->TextState)*(text_splits.size()+1);
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
        painter->TextState.SetFont(*Font, font_size);
        painter->GraphicsState.SetFillColor(PdfColor(color[0], color[1], color[2]));
        auto tmp = convertCoord(coord);
        if (text_splits.empty()) {
            painter->DrawText(text, tmp[0], tmp[1]-font_size);
        } else {
            int last_break = 0;
            for (int i=0; i<text_splits.size(); i++) {
                if (i==0) {
                    painter->DrawText(text.substr(last_break, text_splits[i]), tmp[0], tmp[1]-font_size);
                } else {
                    painter->DrawText(text.substr(last_break, text_splits[i]), margin_x, tmp[1]-font_size*(i+1));
                }
                last_break = text_splits[i];
            }
            painter->DrawText(text.substr(last_break, text.size()), margin_x, tmp[1]-font_size*(text_splits.size()+1));
        }
    } else if (type == Type::RECT) {
        painter->GraphicsState.SetFillColor(PdfColor(bgcolor[0], bgcolor[1], bgcolor[2]));
        painter->GraphicsState.SetStrokeColor(PdfColor(color[0], color[1], color[2]));
        auto tmp = convertCoord(coord);
        painter->DrawRectangle(tmp[0], tmp[1]-size[1], size[0], size[1], PdfPathDrawMode::Fill);
    }
}

PdfGen::PdfGen(vector<shared_ptr<Node>> _nodes) : nodes(_nodes) {}

void PdfGen::genPdf() {
    document = new PdfMemDocument();
    painter = new PdfPainter();
    try {
        page = &(document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)));
        painter->SetCanvas(*page);

        array<double,3> coord = {0.0,0.0,0.0};
        for (shared_ptr<Node> node : nodes) {
            coord = constructObjForNode(node, coord);
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

        document->Save(ArgsParser::output->getName());
    } catch (PdfError &e) {
        try {
            painter->FinishDrawing();
        } catch (...) {}
        throw e;
    }
    delete painter;
    delete document;
}

array<double,3> PdfGen::constructObjForNode(shared_ptr<Node> node, array<double,3> coord) {
    if (PdfGen::objTypes[node->name].type==Object::Type::TEXT) {
        auto obj = make_shared<Object>(Object::Type::TEXT, array<double,2>{coord[0], coord[1]}, node->text, objTypes[node->name].font, objTypes[node->name].font_size, objTypes[node->name].color, objTypes[node->name].bgcolor);
        objs.push_back(obj);
        return array<double,3>{coord[0]+obj->size[0], coord[1]+obj->size[1]-obj->font_size, coord[1]+obj->size[1]};
    } else if (PdfGen::objTypes[node->name].type==Object::Type::RECT) {
        auto obj_coord = array<double,3>{coord[0]+10, coord[1]+10, coord[2]+10};
        for (auto el : node->nodes) {
            auto tmp = constructObjForNode(el, obj_coord);
            obj_coord = array<double,3>{tmp[0], tmp[1], std::max(tmp[2],obj_coord[2])};
        }
        obj_coord = array<double,3>{obj_coord[0]+10, obj_coord[1]+10, obj_coord[2]+10};
        auto obj = make_shared<Object>(Object::Type::RECT, array<double,2>{coord[0], coord[1]}, array<double,2>{obj_coord[0]-coord[0], obj_coord[2]-coord[1]}, PdfGen::objTypes[node->name].color, PdfGen::objTypes[node->name].bgcolor);
        objs.insert(objs.begin(), obj);
        return array<double,3>{obj_coord[0], obj_coord[1]-20, obj_coord[2]};
    } else {
        for (shared_ptr<Node> el : node->nodes) {
            coord = constructObjForNode(el, coord);
        }
        return coord;
    }
}

array<double, 2> PdfGen::convertCoord(array<double, 2> coord) {
    return array<double, 2>{coord[0]+margin_x, page->GetRect().Height-coord[1]-margin_y};
}
