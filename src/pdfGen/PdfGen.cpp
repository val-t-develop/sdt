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
        pages.addPage(&document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)), 1);
        pages.addPage(&document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)), 1);
        painter->SetCanvas(*pages.getActivePage(1));

        map<string, string> root_args{};
        root_args["font"] = "Noto Sans";
        root_args["font_size"] = "14";
        root_args["color"] = "000000";
        root_args["bgcolor"] = "FF0000";
        root_args["doc_margin_x"] = "50";
        root_args["doc_margin_y"] = "50";
        root_args["margin_left"] = "0";
        root_args["margin_right"] = "0";
        root_args["margin_top"] = "0";
        root_args["margin_bottom"] = "0";
        root_args["padding_left"] = "0";
        root_args["padding_right"] = "0";
        root_args["padding_top"] = "0";
        root_args["padding_bottom"] = "0";
        root_args["src"] = ArgsParser::src->getName() + ".png";

        Obj obj = objs[string(reinterpret_cast<const char *>(root->name))];
        for (auto el : obj.args) {
            root_args[el.first] = el.second;
        }

        for (auto attr = root->properties; attr; attr = attr->next) {
            genAttr(attr, root_args);
        }
        array<double, 8> child_pos{stod(root_args["doc_margin_x"]),
                                   0.0,
                                   0.0,
                                   0.0,
                                   0.0,
                                   0.0,
                                   0.0,
                                   pages.getActivePage(1)->GetRect().Width - 2*stod(root_args["doc_margin_x"])};
        for (xmlNode *node = root->children; node; node = node->next) {
            auto t = genNode(node, root_args, child_pos, 1);
            child_pos[2] = t[4];
            child_pos[3] = t[5];
            child_pos[4] = 0;
            child_pos[5] = t[3] + t[1];
        }
        document->GetPages().RemovePageAt(0);

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

array<double, 6> PdfGen::genNode(xmlNode *node, map<string, string> &args, array<double, 8> pos, int render) {
    if (node->type == XML_ELEMENT_NODE) {
        Obj obj = objs[string(reinterpret_cast<const char *>(node->name))];
        map new_args(args);
        for (auto el : obj.args) {
            new_args[el.first] = el.second;
        }

        if (obj.base == "") {
            if (string(reinterpret_cast<const char *>(node->name)) == "__rect") {
                painter->SetCanvas(*pages.getActivePage(0));
                auto new_node = xmlCopyNode(node, 1);
                for (auto attr = new_node->properties; attr; attr = attr->next) {
                    genAttr(attr, new_args);
                }
                double ml = stod(new_args["margin_left"]), mr = stod(new_args["margin_right"]),
                    mt = stod(new_args["margin_top"]), mb = stod(new_args["margin_bottom"]),
                    pl = stod(new_args["padding_left"]), pr = stod(new_args["padding_right"]),
                    pt = stod(new_args["padding_top"]), pb = stod(new_args["padding_bottom"]);
                array<double, 8> child_pos{pos[4]+pos[0]+ml+pl,
                    pos[5]+pos[1]+mt+pt,
                    0.0, 0.0, 0.0, 0.0,
                    pos[6] - pos[4],
                    pos[7] - pos[4]-ml-pl-mr-pr};
                double max_x = child_pos[4];
                new_args["margin_left"] = "0";
                new_args["margin_right"] = "0";
                new_args["margin_top"] = "0";
                new_args["margin_bottom"] = "0";
                new_args["padding_left"] = "0";
                new_args["padding_right"] = "0";
                new_args["padding_top"] = "0";
                new_args["padding_bottom"] = "0";
                for (xmlNode *el = new_node->children; el; el = el->next) {
                    auto t = genNode(el, new_args, child_pos, 0);
                    child_pos[2] = t[4];
                    child_pos[3] = t[5];
                    max_x = max(t[2] + t[0], max_x);
                    child_pos[4] = 0;
                    child_pos[5] = t[3] + t[1];
                }
                painter->SetCanvas(*pages.getActivePage(render));
                drawRect(args, max(pos[4], pos[6])+pos[0]+ml, pos[5]+pos[1]+mt,
                    max_x-pos[4]+pl+pr, child_pos[5]+child_pos[1]-pos[5]-pos[1]-mt+pb,
                    args["bgcolor"], "fill", render);
                for (auto attr = node->properties; attr; attr = attr->next) {
                    genAttr(attr, new_args);
                }
                child_pos = array<double, 8>{pos[4]+pos[0]+ml+pl,
                    pos[5]+pos[1]+mt+pt,
                    0.0, 0.0, 0.0, 0.0,
                    pos[6] - pos[4],
                    pos[7] - pos[4]-ml-pl-mr-pr};

                max_x = child_pos[4];
                new_args["margin_left"] = "0";
                new_args["margin_right"] = "0";
                new_args["margin_top"] = "0";
                new_args["margin_bottom"] = "0";
                new_args["padding_left"] = "0";
                new_args["padding_right"] = "0";
                new_args["padding_top"] = "0";
                new_args["padding_bottom"] = "0";
                for (xmlNode *el = node->children; el; el = el->next) {
                    auto t = genNode(el, new_args, child_pos, render);
                    child_pos[2] = t[4];
                    child_pos[3] = t[5];
                    max_x = max(t[2] + t[0], max_x);
                    child_pos[4] = 0;
                    child_pos[5] = t[3] + t[1];
                }
                return array<double, 6>{max(pos[4], pos[6]),
                                        pos[5],
                                        max_x - pos[4]+ml+mr+pr+pl,
                                        child_pos[5] + child_pos[1] - pos[5] - pos[1]+mt+mb+pt+pb,
                                        0,
                                        child_pos[5]+pos[5]+mt+mb+pt+pb};
            } else if (string(reinterpret_cast<const char *>(node->name)) == "img") {
                auto img = document->CreateImage();
                img->Load(args["src"]);
                painter->DrawImage(*img, max(pos[4], pos[6]) + pos[0],
                                   pages.getActivePage(render)->GetRect().Height - stod(args["doc_margin_y"])
                                   - pos[1] - pos[5] - img->GetHeight());
            }
            // TODO vid
            for (auto attr = node->properties; attr; attr = attr->next) {
                genAttr(attr, new_args);
            }
            array<double, 8> child_pos{pos[4] + pos[0], pos[5] + pos[1], 0.0, 0.0, 0.0, 0.0, pos[6] - pos[4], pos[7] - pos[4]};
            double max_x = child_pos[4];
            for (xmlNode *el = node->children; el; el = el->next) {
                auto t = genNode(el, new_args, child_pos, render);
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
        } else {
            node->name = reinterpret_cast<const xmlChar *>(obj.base.c_str());
            return genNode(node, new_args, pos, render);
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
                                      pages.getActivePage(render)->GetRect().Height - stod(args["doc_margin_y"])  - lines * font->GetLineSpacing(painter->TextState) -
                                          pos[3] - pos[1] - stod(args["font_size"]));
                    last_break = last_space + 1;
                    substr_coord_x = pos[6];
                    lines++;
                }
                last_space = i;
            }
        }
        painter->DrawText(text.substr(last_break, text.length()), pos[0] + substr_coord_x,
                          pages.getActivePage(render)->GetRect().Height - stod(args["doc_margin_y"])  - lines * font->GetLineSpacing(painter->TextState) - pos[1] -
                              pos[3] - stod(args["font_size"]));
        lines++;
        return array<double, 6>{pos[2],
                                pos[3],
                                (lines == 1 ? font->GetStringLength(text, painter->TextState) : pos[7] - pos[6]),
                                lines * font->GetLineSpacing(painter->TextState),
                                font->GetStringLength(text.substr(last_break, text.length()), painter->TextState) +
                                    substr_coord_x,
                                pages.getActivePage(render)->GetRect().Height - stod(args["doc_margin_y"]) - lines * font->GetLineSpacing(painter->TextState) -
                                    pos[3] - stod(args["font_size"])};
    } else {
        Out::errorMessage("Unsupported xml node type");
        return array<double, 6>{-1, -1, -1, -1, -1, -1};
    }
}

void PdfGen::drawRect(map<string, string> &args, double x, double y, double w, double h, string color, string mode, int render) {
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
    if (y+h<(pages.getActivePage(render)->GetRect().Height-stod(args["doc_margin_y"])*2)*pages.active) {
        painter->DrawRectangle(x, pages.getActivePage(render)->GetRect().Height-stod(args["doc_margin_y"])-y-h, w, h, m);
    } else {
        int active_page = pages.active;
        painter->DrawRectangle(x, stod(args["doc_margin_y"]), w, pages.getActivePage(render)->GetRect().Height-y-stod(args["doc_margin_y"])*2, m);
        h -= pages.getActivePage(render)->GetRect().Height-y-stod(args["doc_margin_y"])*2;
        while (h>pages.getActivePage(render)->GetRect().Height-stod(args["doc_margin_y"])*2) {
            pages.addPage(&document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)), render);
            painter->SetCanvas(*pages.getActivePage(render));
            painter->GraphicsState.SetFillColor(PdfColor(col[0], col[1], col[2]));
            painter->DrawRectangle(x, stod(args["doc_margin_y"]), w, pages.getActivePage(render)->GetRect().Height-stod(args["doc_margin_y"])*2, m);
            h -= pages.getActivePage(render)->GetRect().Height-stod(args["doc_margin_y"])*2;
        }
        pages.addPage(&document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)), render);
        painter->SetCanvas(*pages.getActivePage(render));
        painter->GraphicsState.SetFillColor(PdfColor(col[0], col[1], col[2]));
        painter->DrawRectangle(x, pages.getActivePage(render)->GetRect().Height-stod(args["doc_margin_y"])-h, w, h, m);
        pages.active=active_page;
        painter->SetCanvas(*pages.getActivePage(render));
    }

}

void PdfGen::genAttr(xmlAttr *attr, map<string, string> &args) {
    string pname(reinterpret_cast<const char *>(attr->name));
    string pval(reinterpret_cast<char *>(attr->children->content));
    if (pname == "base") {
        while (!pval.empty()) {
            Obj obj = objs[pval];
            for (auto el : obj.args) {
                args[el.first] = el.second;
            }
            pname=obj.base;
        }
    } else if (pname == "font") {
        args["font"] = pval;
    } else if (pname == "font_size") {
        try {
            stod(pval);
            args["font_size"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'font_size' is not numeric");
        }
    } else if (pname == "doc_margin_x") {
        try {
            stod(pval);
            args["doc_margin_x"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'doc_margin_x' is not numeric");
        }
    } else if (pname == "doc_margin_y") {
        try {
            stod(pval);
            args["doc_margin_y"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'doc_margin_y' is not numeric");
        }
    } else if (pname == "margin_left") {
        try {
            stod(pval);
            args["margin_left"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'margin_left' is not numeric");
        }
    } else if (pname == "margin_right") {
        try {
            stod(pval);
            args["margin_right"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'margin_right' is not numeric");
        }
    } else if (pname == "margin_top") {
        try {
            stod(pval);
            args["margin_top"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'margin_top' is not numeric");
        }
    } else if (pname == "margin_bottom") {
        try {
            stod(pval);
            args["margin_bottom"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'margin_bottom' is not numeric");
        }
    } else if (pname == "padding_left") {
        try {
            stod(pval);
            args["padding_left"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'padding_left' is not numeric");
        }
    } else if (pname == "padding_right") {
        try {
            stod(pval);
            args["padding_right"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'padding_right' is not numeric");
        }
    } else if (pname == "padding_top") {
        try {
            stod(pval);
            args["padding_top"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'padding_top' is not numeric");
        }
    } else if (pname == "padding_bottom") {
        try {
            stod(pval);
            args["padding_bottom"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'padding_bottom' is not numeric");
        }
    } else if (pname == "margin_x") {
        try {
            stod(pval);
            args["margin_left"] = pval;
            args["margin_right"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'margin_x' is not numeric");
        }
    } else if (pname == "margin_y") {
        try {
            stod(pval);
            args["margin_top"] = pval;
            args["margin_bottom"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'margin_y' is not numeric");
        }
    } else if (pname == "padding_x") {
        try {
            stod(pval);
            args["padding_left"] = pval;
            args["padding_right"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'padding_x' is not numeric");
        }
    } else if (pname == "padding_y") {
        try {
            stod(pval);
            args["padding_top"] = pval;
            args["padding_bottom"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'padding_y' is not numeric");
        }
    } else if (pname == "margin") {
        try {
            stod(pval);
            args["margin_left"] = pval;
            args["margin_right"] = pval;
            args["margin_top"] = pval;
            args["margin_bottom"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'margin' is not numeric");
        }
    } else if (pname == "padding") {
        try {
            stod(pval);
            args["padding_left"] = pval;
            args["padding_right"] = pval;
            args["padding_top"] = pval;
            args["padding_bottom"] = pval;
        } catch (...) {
            Out::errorMessage("Argument 'padding' is not numeric");
        }
    } else if (pname == "src") {
        args["src"] = pval;
    } else if (pname == "color") {
        if (pval.size() != 6) {
            Out::errorMessage("Argument 'color' is not 6 character string");
        }
        for (auto c : pval) {
            if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                Out::errorMessage(string("Argument 'color' can not contain '") + c + "' character");
            }
        }
        args["color"] = pval;
    } else if (pname == "bgcolor") {
        if (pval.size() != 6) {
            Out::errorMessage("Argument 'bgcolor' is not 6 character string");
        }
        for (auto c : pval) {
            if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
                c != '6' && c != '7' && c != '8' && c != '9' && c != 'A' && c != 'B' &&
                c != 'C' && c != 'D' && c != 'E' && c != 'F') {
                Out::errorMessage(string("Argument 'bgcolor' can not contain '") + c + "' character");
            }
        }
        args["bgcolor"] = pval;
    } else {
        Out::errorMessage("WARNING: argument '" + pname + "' is not known and will be ignored");
    }
}

array<double, 3> PdfGen::genColor(const string &str) {
    unsigned int red, green, blue;
    std::istringstream(str.substr(0, 2)) >> std::hex >> red;
    std::istringstream(str.substr(2, 2)) >> std::hex >> green;
    std::istringstream(str.substr(4, 2)) >> std::hex >> blue;
    return array<double, 3>{red/255.0, green/255.0, blue/255.0};
}