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

map<string, PdfGen::Obj> PdfGen::objs = map<string, PdfGen::Obj>();

PdfGen::PdfGen() {
    xmlInitParser();
    xml_document = xmlReadFile(ArgsParser::src->getName().c_str(), NULL, 0);
}

PdfGen::~PdfGen() {
    xmlCleanupParser();
}

void PdfGen::gen() {
    xmlNode *root = xmlDocGetRootElement(xml_document);
    document = new PdfMemDocument();
    painter = new PdfPainter();

    try {
        page = &(document->GetPages().CreatePage(
            PdfPage::CreateStandardPageSize(PdfPageSize::A4)));
        painter->SetCanvas(*page);

        map<string, string> root_args = map<string, string>();
        root_args["font"] = "Noto Sans";
        root_args["font_size"] = "14";
        root_args["doc_margin_x"] = "50";
        root_args["doc_margin_y"] = "50";
        for (xmlNode *node = root->children; node; node = node->next) {
            genNode(node, root_args, array<double,4>{0.0,0.0,0.0,0.0});
        }

        painter->FinishDrawing();

        // TODO
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
        } catch (...) {
        }
        throw e;
    }

    delete painter;
    delete document;
}

array<double, 6> PdfGen::genNode(xmlNode *node, map<string, string> &args,
                                 array<double, 4> coord) {
    if (node->type == XML_ELEMENT_NODE) {
        Obj obj = objs[string(reinterpret_cast<const char *>(node->name))];
        map<string, string> new_args = map<string, string>(args);
        for (auto el : obj.args) {
            new_args[el.first] = el.second;
        }

        if (obj.base == "") {
            if (string(reinterpret_cast<const char*>(node->name))=="__rect") {

                PdfPage* new_page = &(document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)));
                painter->SetCanvas(*new_page);
                array<double, 4> end = coord;
                for (xmlNode *el = node->children; el; el = el->next) {
                    auto t = genNode(el, new_args, end);
                    end[0] = t[4];
                    end[1] = t[5];
                    end[2] = t[2]+t[0];
                    end[3] = t[3]+t[1];
                }
                document->GetPages().RemovePageAt(new_page->GetIndex());
                painter->SetCanvas(*page);
                painter->GraphicsState.SetFillColor(PdfColor(1.0,0.0,0.0));
                painter->GraphicsState.SetStrokeColor(PdfColor(0.0,1.0,0.0));
                painter->DrawRectangle(coord[0]+stod(args["doc_margin_x"]),
                    page->GetRect().Height-stod(args["doc_margin_y"])-end[3],
                    end[2]-coord[0],
                    end[3]-coord[1],
                    PdfPathDrawMode::Fill);
                end = coord;
                for (xmlNode *el = node->children; el; el = el->next) {
                    auto t = genNode(el, new_args, end);
                    end[0] = t[4];
                    end[1] = t[5];
                    end[2] = t[2]+t[0];
                    end[3] = t[3]+t[1];
                }
                return array<double,6>{coord[0], coord[1], end[2]-coord[0], end[3]-coord[1], end[2], end[3]};
            }
            // TODO img, vid
            return array<double,6>{0.0,0.0,0.0,0.0,0.0,0.0};
        } else {
            node->name = reinterpret_cast<const xmlChar *>(obj.base.c_str());
            return genNode(node, new_args, coord);
        }
    } else if (node->type == XML_ATTRIBUTE_NODE) {
        args[string(reinterpret_cast<const char *>(node->name))] =
            string(reinterpret_cast<char *>(node->content));
        return array<double, 6>{-1, -1, -1, -1, -1, -1};
    } else if (node->type == XML_TEXT_NODE) {
        PdfFont *font = document->GetFonts().SearchFont(args["font"]);
        if (font == nullptr)
            throw std::runtime_error("Invalid handle");

        painter->TextState.SetFont(*font, stod(args["font_size"]));
        painter->GraphicsState.SetFillColor(PdfColor(0.0,0.0,0.0));
        string text = string(reinterpret_cast<char *>(node->content));

        int last_space = 0, last_break = 0, substr_coord_x = coord[0], lines = 0;
        for (int i=0; i<text.length(); i++) {
            if (text[i] == ' ') {
                string substr = text.substr(last_break, i-last_break+1);
                if (font->GetStringLength(substr, painter->TextState)+substr_coord_x>page->GetRect().Width-stod(args["doc_margin_x"])*2) {
                    string s = text.substr(last_break,last_space-last_break+1);
                    painter->DrawText(s, substr_coord_x+stod(args["doc_margin_x"]), page->GetRect().Height-(lines+1)*font->GetLineSpacing(painter->TextState)-coord[1]-stod(args["doc_margin_y"]));
                    last_break = last_space+1;
                    substr_coord_x = 0;
                    lines++;
                }
                last_space = i;
            }
        }
        painter->DrawText(text.substr(last_break,text.length()), substr_coord_x+stod(args["doc_margin_x"]), page->GetRect().Height-(lines+1)*font->GetLineSpacing(painter->TextState)-coord[1]-stod(args["doc_margin_y"]));
        lines++;
        return array<double, 6>{coord[0], coord[1],
            (lines==1 ? font->GetStringLength(text, painter->TextState) : page->GetRect().Width-stod(args["doc_margin_x"])*2),
            (lines+1)*font->GetLineSpacing(painter->TextState),
            font->GetStringLength(text.substr(last_break,text.length()), painter->TextState)+stod(args["doc_margin_x"])+substr_coord_x,
            page->GetRect().Height-lines*font->GetLineSpacing(painter->TextState)-coord[1]-stod(args["doc_margin_y"])};
    } else {
        Out::errorMessage("Unsupported xml node type");
        return array<double, 6>{-1, -1, -1, -1, -1, -1};
    }
}