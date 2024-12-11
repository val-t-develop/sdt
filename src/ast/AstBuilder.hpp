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
#pragma once
#include <utils/Path.hpp>
#include <ast/Node.hpp>

class AstBuilder {
public:
    Path path;
    string src;
    vector<shared_ptr<Node>> nodes;
    int currChar = 0;
    int currLine = 0, currPos = 0;

    AstBuilder();
    AstBuilder(Path &filePath);

    void buildNodes();
    vector<shared_ptr<Node>> buildNodesList();

    void incCurrChar();
};