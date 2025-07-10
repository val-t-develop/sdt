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

#include "utils/ArgsParser.hpp"

XSLGen::XSLGen() {
    xml_document = xmlNewDoc(BAD_CAST "1.0");
    xsl_document = xmlNewDoc(BAD_CAST "1.0");

    xsl_stylesheet = xmlNewNode(NULL, BAD_CAST "xsl:stylesheet");
    xmlNewProp(xsl_stylesheet, BAD_CAST "version", BAD_CAST "1.0");
    xmlNewProp(xsl_stylesheet, BAD_CAST "xmlns:xsl", BAD_CAST "http://www.w3.org/1999/XSL/Transform");
    xmlNewProp(xsl_stylesheet, BAD_CAST "xmlns:fo", BAD_CAST "http://www.w3.org/1999/XSL/Format");
    xmlDocSetRootElement(xsl_document, xsl_stylesheet);

    xmlNode *xsl_output = xmlNewNode(NULL, BAD_CAST "xsl:output");
    xmlNewProp(xsl_output, BAD_CAST "method", BAD_CAST "xml");
    xmlNewProp(xsl_output, BAD_CAST "indent", BAD_CAST "yes");
    xmlAddChild(xsl_stylesheet, xsl_output);
}

XSLGen::~XSLGen() {
    FILE *xml_file = fopen((ArgsParser::src->getName()+".xml").c_str(), "w");
    FILE *xsl_file = fopen((ArgsParser::src->getName()+".xsl").c_str(), "w");
    if ((!xml_file) || (!xsl_file)) {
        cerr << "Can not open file" << endl;
    }
    xmlDocDump(xml_file, xml_document);
    xmlDocDump(xsl_file, xsl_document);
    fclose(xml_file);
    fclose(xsl_file);
    xmlCleanupParser();
}

size_t XSLGen::addBlock(size_t parent, map<string, string> attrs) {
    size_t id = xml_nodes.size();

    xmlNode *xml_node = xmlNewNode(NULL, BAD_CAST("block" + to_string(id)).c_str());
    if (id == 0) {
        xmlDocSetRootElement(xml_document, xml_node);
    } else {
        xmlAddChild(xml_nodes[parent], xml_node);
    }
    xml_nodes.push_back(xml_node);

    xmlNode *xsl_template = xmlNewNode(NULL, BAD_CAST "xsl:template");
    xmlNewProp(xsl_template, BAD_CAST "match", BAD_CAST("block" + to_string(id)).c_str());
    xmlAddChild(xmlDocGetRootElement(xsl_document), xsl_template);

    if (id == 0) {
        xmlNode *xsl_root = xmlNewNode(NULL, BAD_CAST "fo:root");
        xmlNewProp(xsl_root, BAD_CAST "xmlns:fo", BAD_CAST "http://www.w3.org/1999/XSL/Format");
        xmlAddChild(xsl_template, xsl_root);

        xmlNode *xsl_layout_master_set = xmlNewNode(NULL, BAD_CAST "fo:layout-master-set");
        xmlAddChild(xsl_root, xsl_layout_master_set);

        xmlNode *xsl_simple_page_master = xmlNewNode(NULL, BAD_CAST "fo:simple-page-master");
        xmlNewProp(xsl_simple_page_master, BAD_CAST "master-name", BAD_CAST "A4");
        xmlNewProp(xsl_simple_page_master, BAD_CAST "page-width", BAD_CAST "21cm");
        xmlNewProp(xsl_simple_page_master, BAD_CAST "page-height", BAD_CAST "29.7cm");
        xmlNewProp(xsl_simple_page_master, BAD_CAST "margin", BAD_CAST "2cm");
        xmlAddChild(xsl_layout_master_set, xsl_simple_page_master);

        xmlNode *xsl_region_body = xmlNewNode(NULL, BAD_CAST "fo:region-body");
        xmlAddChild(xsl_simple_page_master, xsl_region_body);

        xmlNode *xsl_page_sequence = xmlNewNode(NULL, BAD_CAST "fo:page-sequence");
        xmlNewProp(xsl_page_sequence, BAD_CAST "master-reference", BAD_CAST "A4");
        xmlAddChild(xsl_root, xsl_page_sequence);

        xmlNode *xsl_flow = xmlNewNode(NULL, BAD_CAST "fo:flow");
        xmlNewProp(xsl_flow, BAD_CAST "flow-name", BAD_CAST "xsl-region-body");
        xmlAddChild(xsl_page_sequence, xsl_flow);

        xmlNode *xsl_apply_templates = xmlNewNode(NULL, BAD_CAST "xsl:apply-templates");
        xmlAddChild(xsl_flow, xsl_apply_templates);
    } else {
        if (attrs.contains("width") || attrs.contains("height")) {
            xmlNode *xsl_block_container = xmlNewNode(NULL, BAD_CAST "fo:block-container");
            addContainerProps(xsl_block_container, attrs);
            xmlAddChild(xsl_template, xsl_block_container);

            xmlNode *xsl_block = xmlNewNode(NULL, BAD_CAST "fo:block");
            addProps(xsl_block, attrs);
            xmlAddChild(xsl_block_container, xsl_block);
            xsl_nodes.push_back(xsl_block);

            xmlNode *xsl_apply_templates = xmlNewNode(NULL, BAD_CAST "xsl:apply-templates");
            xmlAddChild(xsl_block, xsl_apply_templates);
        } else {
            xmlNode *xsl_block = xmlNewNode(NULL, BAD_CAST "fo:block");
            addProps(xsl_block, attrs);
            xmlAddChild(xsl_template, xsl_block);
            xsl_nodes.push_back(xsl_block);

            xmlNode *xsl_apply_templates = xmlNewNode(NULL, BAD_CAST "xsl:apply-templates");
            xmlAddChild(xsl_block, xsl_apply_templates);
        }
    }
    return id;
}

void XSLGen::addText(size_t parent, string text) {
    size_t id = xml_nodes.size();
    xmlNode *xml_node = xmlNewNode(NULL, BAD_CAST("text" + to_string(id)).c_str());
    if (id == 0) {
        xmlDocSetRootElement(xml_document, xml_node);
    } else {
        xmlAddChild(xml_nodes[parent], xml_node);
    }
    xml_nodes.push_back(xml_node);

    xmlNode *xml_text = xmlNewText(BAD_CAST text.c_str());
    xmlAddChild(xml_node, xml_text);

    xmlNode *xsl_template = xmlNewNode(NULL, BAD_CAST "xsl:template");
    xmlNewProp(xsl_template, BAD_CAST "match", BAD_CAST("text" + to_string(id)).c_str());
    xmlAddChild(xmlDocGetRootElement(xsl_document), xsl_template);

    xmlNode *xsl_block = xmlNewNode(NULL, BAD_CAST "fo:block");
    xmlAddChild(xsl_template, xsl_block);
    xsl_nodes.push_back(xsl_block);

    xmlNode *xsl_apply_templates = xmlNewNode(NULL, BAD_CAST "xsl:apply-templates");
    xmlAddChild(xsl_block, xsl_apply_templates);
}

void XSLGen::addImage(size_t parent, map<string, string> attrs) {
    size_t id = xml_nodes.size();
    xmlNode *xml_node = xmlNewNode(NULL, BAD_CAST("image" + to_string(id)).c_str());
    xmlNewProp(xml_node, BAD_CAST "src", BAD_CAST attrs["src"].c_str());
    if (id == 0) {
        xmlDocSetRootElement(xml_document, xml_node);
    } else {
        xmlAddChild(xml_nodes[parent], xml_node);
    }
    xml_nodes.push_back(xml_node);

    xmlNode *xsl_template = xmlNewNode(NULL, BAD_CAST "xsl:template");
    xmlNewProp(xsl_template, BAD_CAST "match", BAD_CAST("image" + to_string(id)).c_str());
    xmlAddChild(xmlDocGetRootElement(xsl_document), xsl_template);

    xmlNode *xsl_block = xmlNewNode(NULL, BAD_CAST "fo:block");
    xmlAddChild(xsl_template, xsl_block);
    xsl_nodes.push_back(xsl_block);

    xmlNode *xsl_external_graphic = xmlNewNode(NULL, BAD_CAST "fo:external-graphic");
    xmlNewProp(xsl_external_graphic, BAD_CAST "src", BAD_CAST "url('{@src}')");
    xmlAddChild(xsl_block, xsl_external_graphic);
}

void XSLGen::addProps(xmlNode *node, map<string, string> attrs) {
    bool left = false, right = false, top = false, bottom = false;
    for (auto prop : attrs) {
        if (prop.first == "font") {
            xmlNewProp(node, BAD_CAST "font-family", BAD_CAST prop.second.c_str());
        } else if (prop.first == "font-size") {
            xmlNewProp(node, BAD_CAST "font-size", BAD_CAST(prop.second + "pt").c_str());
        } else if (prop.first == "font-color") {
            xmlNewProp(node, BAD_CAST "color", BAD_CAST("#" + prop.second).c_str()); // TODO # to ConfReader
        } else if (prop.first == "text-align") {
            xmlNewProp(node, BAD_CAST "text-align", BAD_CAST prop.second.c_str());
        } else if (prop.first == "background-color") {
            xmlNewProp(node, BAD_CAST "background-color", BAD_CAST("#" + prop.second).c_str());
        } else if (prop.first == "left-border-color" || prop.first == "left-border-width" ||
                   prop.first == "left-border-style") {
            if (!left) {
                string width = attrs.contains("left-border-width") ? attrs["left-border-width"] : "0";
                string style = attrs.contains("left-border-style") ? attrs["left-border-style"] : "solid";
                string color = attrs.contains("left-border-color") ? attrs["left-border-color"] : "000000";
                xmlNewProp(node, BAD_CAST "border-left", BAD_CAST(width + "pt " + style + " #" + color).c_str());
                left = true;
            }
        } else if (prop.first == "right-border-color" || prop.first == "right-border-width" ||
                   prop.first == "right-border-style") {
            if (!right) {
                string width = attrs.contains("right-border-width") ? attrs["right-border-width"] : "0";
                string style = attrs.contains("right-border-style") ? attrs["right-border-style"] : "solid";
                string color = attrs.contains("right-border-color") ? attrs["right-border-color"] : "000000";
                xmlNewProp(node, BAD_CAST "border-right", BAD_CAST(width + "pt " + style + " #" + color).c_str());
                right = true;
            }
        } else if (prop.first == "top-border-color" || prop.first == "top-border-width" ||
                   prop.first == "top-border-style") {
            if (!top) {
                string width = attrs.contains("top-border-width") ? attrs["top-border-width"] : "0";
                string style = attrs.contains("top-border-style") ? attrs["top-border-style"] : "solid";
                string color = attrs.contains("top-border-color") ? attrs["top-border-color"] : "000000";
                xmlNewProp(node, BAD_CAST "border-top", BAD_CAST(width + "pt " + style + " #" + color).c_str());
                top = true;
            }
        } else if (prop.first == "bottom-border-color" || prop.first == "bottom-border-width" ||
                   prop.first == "bottom-border-style") {
            if (!bottom) {
                string width = attrs.contains("bottom-border-width") ? attrs["bottom-border-width"] : "0";
                string style = attrs.contains("bottom-border-style") ? attrs["bottom-border-style"] : "solid";
                string color = attrs.contains("bottom-border-color") ? attrs["bottom-border-color"] : "000000";
                xmlNewProp(node, BAD_CAST "border-bottom", BAD_CAST(width + "pt " + style + " #" + color).c_str());
                bottom = true;
            }
        } else if (prop.first == "margin-left" || prop.first == "margin-right" || prop.first == "margin-top" ||
                   prop.first == "margin-bottom" || prop.first == "padding-left" || prop.first == "padding-right" ||
                   prop.first == "padding-top" || prop.first == "padding-bottom") {
            xmlNewProp(node, BAD_CAST prop.first.c_str(), BAD_CAST(prop.second + "pt").c_str());
        }
    }
}

void XSLGen::addContainerProps(xmlNode *node, map<string, string> attrs) {
    for (auto prop : attrs) {
        if (prop.first == "width") {
            double width = std::stod(prop.second);
            double margin_left = !attrs.contains("margin-left") ? 0 : std::stod(attrs["margin-left"]);
            double margin_right = !attrs.contains("margin-right") ? 0 : std::stod(attrs["margin-right"]);
            xmlNewProp(node, BAD_CAST prop.first.c_str(), BAD_CAST(to_string(width+margin_left+margin_right)+"pt").c_str());
        } else if (prop.first == "height") {
            double height = std::stod(prop.second);
            double margin_top = !attrs.contains("margin-top") ? 0 : std::stod(attrs["margin-top"]);
            double margin_bottom = !attrs.contains("margin-bottom") ? 0 : std::stod(attrs["margin-bottom"]);
            xmlNewProp(node, BAD_CAST prop.first.c_str(), BAD_CAST(to_string(height+margin_top+margin_bottom)+"pt").c_str());
        }
    }

}
