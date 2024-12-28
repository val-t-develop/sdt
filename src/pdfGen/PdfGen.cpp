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
#include <boost/algorithm/string.hpp>
#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>

map<string, PdfGen::Obj> PdfGen::objs = map<string, Obj>();

PdfGen::PdfGen() {
    xmlInitParser();
    xml_document = xmlReadFile(ArgsParser::src->getName().c_str(), nullptr, XML_PARSE_NOBLANKS);
}

PdfGen::~PdfGen() { xmlCleanupParser(); }

void PdfGen::gen() {
    const xmlNode *root = xmlDocGetRootElement(xml_document);
    document = new PdfMemDocument();
    painter = new PdfPainter();

    try {
        pages.push(&document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)));
        painter->SetCanvas(*pages.top());

        map<string, string> root_args{};
        root_args["font"] = "Noto Sans";
        root_args["font_size"] = "14";
        root_args["color"] = "000000";
        root_args["bgcolor"] = "FF0000";
        root_args["doc_margin_x"] = "50";
        root_args["doc_margin_y"] = "50";
        root_args["src"] = ArgsParser::src->getName() + ".png";

        Obj obj = objs[string(reinterpret_cast<const char *>(root->name))];
        for (auto el : obj.args) {
            root_args[el.first] = el.second;
        }

        for (auto attr = root->properties; attr; attr = attr->next) {
            genAttr(attr, root_args);
        }
        array<double, 8> child_pos{stod(root_args["doc_margin_x"]),
                                   stod(root_args["doc_margin_y"]),
                                   0.0,
                                   0.0,
                                   0.0,
                                   0.0,
                                   0.0,
                                   pages.top()->GetRect().Width - 2*stod(root_args["doc_margin_x"])};
        for (xmlNode *node = root->children; node; node = node->next) {
            auto t = genNode(node, root_args, child_pos);
            child_pos[2] = t[4];
            child_pos[3] = t[5];
            child_pos[4] = 0;
            child_pos[5] = t[3] + t[1];
        }

        painter->FinishDrawing();

        // TODO
        document->GetMetadata().SetCreator(PdfString("SDT - Scientific Document Tool (powered by PoDoFo, libxml2, boost)"));
        document->GetMetadata().SetAuthor(PdfString(""));
        document->GetMetadata().SetTitle(PdfString(""));
        document->GetMetadata().SetSubject(PdfString(""));
        document->GetMetadata().SetKeywords(vector<string>({""}));

        document->Save(ArgsParser::output->getName());
    } catch (PdfError& e) {
        try {
            painter->FinishDrawing();
        } catch (...) {}
        throw e;
    }

    delete painter;
    delete document;
}

array<double, 6> PdfGen::genNode(xmlNode *node, map<string, string> &args, array<double, 8> pos) {
    if (node->type == XML_ELEMENT_NODE) {
        Obj obj = objs[string(reinterpret_cast<const char *>(node->name))];
        map new_args(args);
        for (auto el : obj.args) {
            new_args[el.first] = el.second;
        }

        if (obj.base == "") {
            if (string(reinterpret_cast<const char *>(node->name)) == "__rect") {
                pages.push(&document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)));
                painter->SetCanvas(*pages.top());
                auto new_node = xmlCopyNode(node, 1);
                for (auto attr = new_node->properties; attr; attr = attr->next) {
                    genAttr(attr, new_args);
                }
                array<double, 8> child_pos{pos[4]+pos[0], pos[5]+pos[1], 0.0, 0.0, 0.0, 0.0, pos[6] - pos[4], pos[7] - pos[4]};
                double max_x = child_pos[4];

                for (xmlNode *el = new_node->children; el; el = el->next) {
                    auto t = genNode(el, new_args, child_pos);
                    child_pos[2] = t[4];
                    child_pos[3] = t[5];
                    max_x = max(t[2] + t[0], max_x);
                    child_pos[4] = 0;
                    child_pos[5] = t[3] + t[1];
                }
                document->GetPages().RemovePageAt(pages.top()->GetIndex());
                pages.pop();
                painter->SetCanvas(*pages.top());
                drawRect(max(pos[4], pos[6])+pos[0], pos[5]+pos[1],
                    max_x - pos[4], child_pos[5] + child_pos[1] - pos[5] - pos[1],
                    args["bgcolor"], "fill");
                for (auto attr = node->properties; attr; attr = attr->next) {
                    genAttr(attr, new_args);
                }
                child_pos = array<double, 8>{pos[4]+pos[0], pos[5]+pos[1], 0.0, 0.0, 0.0, 0.0, pos[6] - pos[4], pos[7] - pos[4]};
                max_x = child_pos[4];
                for (xmlNode *el = node->children; el; el = el->next) {
                    auto t = genNode(el, new_args, child_pos);
                    child_pos[2] = t[4];
                    child_pos[3] = t[5];
                    max_x = max(t[2] + t[0], max_x);
                    child_pos[4] = 0;
                    child_pos[5] = t[3] + t[1];
                }
                return array<double, 6>{max(pos[4], pos[6]),
                                        pos[5],
                                        max_x - pos[4],
                                        child_pos[5] + child_pos[1] - pos[5] - pos[1],
                                        0,
                                        child_pos[5]+pos[5]};
            } else if (string(reinterpret_cast<const char *>(node->name)) == "img") {
                auto img = document->CreateImage();
                img->Load(args["src"]);
                painter->DrawImage(*img, max(pos[4], pos[6]) + pos[0],
                                   pages.top()->GetRect().Height - pos[1] - pos[5] -
                                       img->GetHeight());
            }
            // TODO vid
            for (auto attr = node->properties; attr; attr = attr->next) {
                genAttr(attr, new_args);
            }
            array<double, 8> child_pos{pos[4], pos[5], 0.0, 0.0, 0.0, 0.0, pos[6] - pos[4], pos[7] - pos[4]};
            double max_x = child_pos[4];
            for (xmlNode *el = node->children; el; el = el->next) {
                auto t = genNode(el, new_args, child_pos);
                child_pos[2] = t[4];
                child_pos[3] = t[5];
                max_x = max(t[2] + t[0], max_x);
                child_pos[4] = 0;
                child_pos[5] = t[3] + t[1];
            }
            return array<double, 6>{pos[2],
                                    pos[3],
                                    child_pos[4] + child_pos[0] - pos[2],
                                    child_pos[5] + child_pos[1] - pos[3],
                                    0,
                                    child_pos[5] + child_pos[1]};
        } else {
            node->name = reinterpret_cast<const xmlChar *>(obj.base.c_str());
            return genNode(node, new_args, pos);
        }
    } else if (node->type == XML_TEXT_NODE) {
        PdfFont *font = document->GetFonts().SearchFont(args["font"]);
        if (font == nullptr)
            throw std::runtime_error("Invalid handle");

        painter->TextState.SetFont(*font, stod(args["font_size"]));
        auto col = genColor(args["color"]);
        painter->GraphicsState.SetFillColor(PdfColor(col[0], col[1], col[2]));
        string text(reinterpret_cast<char *>(node->content));
        text.erase(std::ranges::remove(text, '\n').begin(), text.end());
        boost::trim(text);
        if (text.empty()) {
            return array<double, 6>{pos[2], pos[3], 0.0, 0.0, pos[2], pos[3]};
        }
        int last_space = 0, last_break = 0, substr_coord_x = max(pos[2], pos[6]), lines = 0;
        for (int i = 0; i < text.length(); i++) {
            if (text[i] == ' ') {
                string substr = text.substr(last_break, i - last_break + 1);
                if (font->GetStringLength(substr, painter->TextState) + substr_coord_x > pos[7]) {
                    string s = text.substr(last_break, last_space - last_break + 1);
                    painter->DrawText(s, pos[0] + substr_coord_x,
                                      pages.top()->GetRect().Height - lines * font->GetLineSpacing(painter->TextState) -
                                          pos[3] - pos[1] - stod(args["font_size"]));
                    last_break = last_space + 1;
                    substr_coord_x = pos[6];
                    lines++;
                }
                last_space = i;
            }
        }
        painter->DrawText(text.substr(last_break, text.length()), pos[0] + substr_coord_x,
                          pages.top()->GetRect().Height - lines * font->GetLineSpacing(painter->TextState) - pos[1] -
                              pos[3] - stod(args["font_size"]));
        lines++;
        return array<double, 6>{pos[2],
                                pos[3],
                                (lines == 1 ? font->GetStringLength(text, painter->TextState) : pos[7] - pos[6]),
                                lines * font->GetLineSpacing(painter->TextState),
                                font->GetStringLength(text.substr(last_break, text.length()), painter->TextState) +
                                    substr_coord_x,
                                pages.top()->GetRect().Height - lines * font->GetLineSpacing(painter->TextState) -
                                    pos[3] - stod(args["font_size"])};
    } else {
        Out::errorMessage("Unsupported xml node type");
        return array<double, 6>{-1, -1, -1, -1, -1, -1};
    }
}

void PdfGen::drawRect(double x, double y, double w, double h, string color, string mode = "fill") {
    auto col = genColor(color);
    PdfPathDrawMode m;
    if (mode == "fill") {
        m = PdfPathDrawMode::Fill;
    } else if (mode == "stroke") {
        m = PdfPathDrawMode::Stroke;
    } else {
        Out::errorMessage("Rectangle draw mode is unknown");
    }
    painter->GraphicsState.SetFillColor(PdfColor(col[0], col[1], col[2]));
    painter->DrawRectangle(x, pages.top()->GetRect().Height-y-h, w, h, m);
}

void PdfGen::genAttr(xmlAttr *attr, map<string, string> &args) {
    args[string(reinterpret_cast<const char *>(attr->name))] =
        string(reinterpret_cast<char *>(attr->children->content));
}

array<double, 3> PdfGen::genColor(const string &str) {
    unsigned int red, green, blue;
    std::istringstream(str.substr(0, 2)) >> std::hex >> red;
    std::istringstream(str.substr(2, 2)) >> std::hex >> green;
    std::istringstream(str.substr(4, 2)) >> std::hex >> blue;
    return array<double, 3>{red/255.0, green/255.0, blue/255.0};
}