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
 *  Builder of AST.
 */
#include "AstBuilder.hpp"

#include <utils/ArgsParser.hpp>
#include <utils/Out.hpp>

AstBuilder::AstBuilder() {
    nodes = vector<shared_ptr<Node>>();
    path = ArgsParser::src;
    src = path->readFile();
}

void AstBuilder::buildNodes() {
    nodes=buildNodesList();
}

vector<shared_ptr<Node>> AstBuilder::buildNodesList() {
    vector<shared_ptr<Node>> retNodes = vector<shared_ptr<Node>>();
    while (true) {
        char ch = '\0', ch1 = '\0';
        try {
            ch = src.at(currChar);
            ch1 = src.at(currChar + 1);
        } catch (std::out_of_range &ignored) {}

        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
            incCurrChar();
            continue;
        }

        if (ch == '/' && ch1 == '/') {
            while (src.at(currChar) != '\n') {
                incCurrChar();
            }
            continue;
        }

        if (ch == '/' && ch1 == '*') {
            while (true) {
                if (src.at(currChar) != '/' && src.at(currChar - 1) != '*') {
                    incCurrChar();
                    break;
                }
                incCurrChar();
            }
            continue;
        }

        if (ch == '\0') {
            break;
        } else if (ch == '<' && ch1 != '\\') {
            incCurrChar();
            string str;
             while (true) {
                try {
                    ch = src.at(currChar);
                    ch1 = src.at(currChar + 1);
                } catch (std::out_of_range &ignored) {}
                if (!isalpha(ch) && !isdigit(ch) && ch!='_') {
                    break;
                }
                str.push_back(ch);
                incCurrChar();
            }

            try {
                ch = src.at(currChar);
                ch1 = src.at(currChar + 1);
            } catch (std::out_of_range &ignored) {}
            if (ch=='>') {
                incCurrChar();
            }
            retNodes.push_back(make_shared<Node>(str, buildNodesList(), ""));
            try {
                ch = src.at(currChar);
                ch1 = src.at(currChar + 1);
            } catch (std::out_of_range &ignored) {}
            if (ch == '<' && ch1 == '\\') {
                incCurrChar();
                incCurrChar();
                string str1;
                while (true) {
                    try {
                        ch = src.at(currChar);
                        ch1 = src.at(currChar + 1);
                    } catch (std::out_of_range &ignored) {}
                    if (!isalpha(ch) && !isdigit(ch) && ch!='_') {
                        break;
                    }
                    str1.push_back(ch);
                    incCurrChar();
                }

                try {
                    ch = src.at(currChar);
                    ch1 = src.at(currChar + 1);
                } catch (std::out_of_range &ignored) {}
                if (ch=='>') {
                    incCurrChar();
                }
                if (str!=str1) {
                    Out::errorMessage("Something went wrong");
                }
            }
        } else if (ch == '<' && ch1 == '\\') {
            break;
        } else if (ch == '{') {
            Out::errorMessage("{} are unsupported now");
        } else {
            string str;
            while (true) {
                ch = '\0', ch1 = '\0';
                try {
                    ch = src.at(currChar);
                    ch1 = src.at(currChar + 1);
                } catch (std::out_of_range &ignored) {}
                if ((ch == '\\' && ch1 == '<') || (ch == '\\' && ch1 == '{')) {
                    str.push_back(ch1);
                    incCurrChar();
                    incCurrChar();
                } else if (ch == '\0') {
                    retNodes.push_back(make_shared<Node>("text", vector<shared_ptr<Node>>(), str));
                    break;
                } else if (ch != '<' && ch != '{') {
                    str.push_back(ch);
                    incCurrChar();
                } else {
                    retNodes.push_back(make_shared<Node>("text", vector<shared_ptr<Node>>(), str));
                    break;
                }
            }
        }
    }
    return retNodes;
}

void AstBuilder::incCurrChar() {
    if (src.at(currChar) == '\n') {
        currLine++;
        currPos = 1;
    } else {
        currPos++;
    }
    currChar++;
}