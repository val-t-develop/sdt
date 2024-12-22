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
    xmlFreeDoc(xml_document);
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
            genNode(node, root_args, array<double,3>{0.0,0.0,0.0});
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

array<double, 3> PdfGen::genNode(xmlNode *node, map<string, string> &args,
                                 array<double, 3> coord) {
    if (node->type == XML_ELEMENT_NODE) {
        Obj obj = objs[string(reinterpret_cast<const char *>(node->name))];
        map<string, string> new_args = map<string, string>(args);
        for (auto el : obj.args) {
            new_args[el.first] = el.second;
        }
        for (xmlNode *el = node->children; node; node = node->next) {
            genNode(el, new_args, array<double, 3>{0.0, 0.0, 0.0});
        }
        if (obj.base == "") {
            // TODO rect, img, vid
        } else {
            node->name = reinterpret_cast<const xmlChar *>(obj.base.c_str());
            return genNode(node, new_args, coord);
        }
    } else if (node->type == XML_ATTRIBUTE_NODE) {
        args[string(reinterpret_cast<const char *>(node->name))] =
            string(reinterpret_cast<char *>(node->content));
        return array<double, 3>{-1, -1, -1};
    } else if (node->type == XML_TEXT_NODE) {
        PdfFont *font = document->GetFonts().SearchFont(args["font"]);
        if (font == nullptr)
            throw std::runtime_error("Invalid handle");

        painter->TextState.SetFont(*font, stod(args["font_size"]));
        // painter->GraphicsState.SetFillColor(PdfColor());
        string text = string(reinterpret_cast<char *>(node->content));

        vector<int> text_splits{};
        int last_space = 0, last_break = 0;
        for (int i = 0; i < text.size(); i++) {
            if (text[i] == ' ') {
                string substr = text.substr(last_break, i - last_break + 1);
                double l = font->GetStringLength(substr, painter->TextState);
                if (last_space == 0) {
                    if (coord[0] + l > page->GetRect().Width -
                                           stod(args["doc_margin_x"]) * 2) {
                        last_break = last_space;
                        text_splits.push_back(last_break);
                    }
                } else {
                    if (l > page->GetRect().Width -
                                stod(args["doc_margin_x"]) * 2) {
                        last_break = last_space;
                        text_splits.push_back(last_break);
                    }
                }
                last_space = i;
            }
        }
        array<double, 2> size{-1, -1};
        if (text_splits.empty()) {
            size[0] = font->GetStringLength(text, painter->TextState);
        } else {
            size[0] = font->GetStringLength(
                          text.substr(text_splits[text_splits.size() - 1],
                                      text.size()),
                          painter->TextState) -
                      coord[0];
        }
        size[1] =
            font->GetLineSpacing(painter->TextState) * (text_splits.size() + 1);
        auto tmp = convertCoord(array<double,2>{coord[0], coord[1]}, args);
        if (text_splits.empty()) {
            painter->DrawText(text, tmp[0], tmp[1] - stod(args["font_size"]));
        } else {
            int last_break = 0;
            for (int i = 0; i < text_splits.size(); i++) {
                if (i == 0) {
                    painter->DrawText(text.substr(last_break, text_splits[i]+1),
                                      tmp[0], tmp[1] - stod(args["font_size"]));
                } else {
                    painter->DrawText(text.substr(last_break+1, text_splits[i]+1),
                                      stod(args["doc_margin_x"]),
                                      tmp[1] -
                                          stod(args["font_size"]) * (i + 1));
                }
                last_break = text_splits[i];
            }
            painter->DrawText(text.substr(last_break+1, text.size()),
                              stod(args["doc_margin_x"]),
                              tmp[1] - stod(args["font_size"]) *
                                           (text_splits.size() + 1));
        }
        return array<double, 3>{coord[0] + size[0],
                                coord[1] + size[1] - stod(args["font_size"]),
                                coord[1] + size[1]};
    } else {
        Out::errorMessage("Unsupported xml node type");
        return coord;
    }
}

array<double, 2> PdfGen::convertCoord(array<double,2> coord, map<string, string> &args) {
    return array<double, 2>{coord[0]+stod(args["doc_margin_x"]), page->GetRect().Height-coord[1]-stod(args["doc_margin_x"])};
}