/*  SDT - Scientific Document Tool
 *  Copyright (C) 2022-2025  Valentyn Tymchyshyn
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
 *  Generator of xsl.
 */
#pragma once
#include <Defs.hpp>
#include <libxml/parser.h>

class XSLGen {
    xmlNode *xsl_stylesheet;
public:
    xmlDoc *xml_document;
    xmlDoc *xsl_document;
    vector<xmlNode*> xml_nodes = vector<xmlNode*>();
    vector<xmlNode*> xsl_nodes = vector<xmlNode*>();

    XSLGen();
    ~XSLGen();

    size_t addBlock(size_t parent, const map<string, string>& attrs);
    void addText(size_t parent, const string& text);
    void addImage(size_t parent, map<string, string> attrs);
    void addBr(size_t parent, const map<string, string>& attrs);
    void addPbr(size_t parent, const map<string, string>& attrs) const;
    size_t addTable(size_t parent, const map<string, string> &attrs);
    size_t addRow(size_t parent, const map<string, string> &attrs);
    size_t addCell(size_t parent, const map<string, string> &attrs);

    static void addProps(xmlNode *node, map<string, string> attrs);
    static void addContainerProps(xmlNode *node, map<string, string> attrs);
};