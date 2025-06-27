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
#include "XSLGen.hpp"

XSLGen::XSLGen() {
    xml_document = xmlNewDoc(BAD_CAST "1.0");
    xsl_document = xmlNewDoc(BAD_CAST "1.0");

    xmlNode *xsl_stylesheet = xmlNewNode(NULL, BAD_CAST "xsl:stylesheet");
    xmlNewProp(xsl_stylesheet, BAD_CAST "version", BAD_CAST "1.0");
    xmlNewProp(xsl_stylesheet, BAD_CAST "xmlns:xsl", BAD_CAST "http://www.w3.org/1999/XSL/Transform");
    xmlNewProp(xsl_stylesheet, BAD_CAST "xmlns:fo", BAD_CAST "http://www.w3.org/1999/XSL/Format");
    xmlDocSetRootElement(xsl_document, xsl_stylesheet);
    xsl_nodes.push_back(xsl_stylesheet);

    xmlNode *xsl_output = xmlNewChild(xsl_stylesheet, NULL, BAD_CAST "xsl:output", NULL);
    xmlNewProp(xsl_output, BAD_CAST "method", BAD_CAST "xml");
    xmlNewProp(xsl_output, BAD_CAST "indent", BAD_CAST "yes");
    xsl_nodes.push_back(xsl_output);

}

XSLGen::~XSLGen() {
    xmlSaveFormatFileEnc("sdt_input.xml", xml_document, "UTF-8", 1);
    xmlSaveFormatFileEnc("sdt_transform.xsl", xsl_document, "UTF-8", 1);
    // Free the document
    // xmlFreeDoc(xml_document);
    // xmlFreeDoc(xsl_document);
    xmlCleanupParser();
}

size_t XSLGen::addNode(size_t parent, string name, vector<pair<string, string>> attrs) {

}
