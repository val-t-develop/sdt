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
 *  Filesystem helper file.
 */
#include "Path.hpp"

Path::Path() {
    path = fs::path();
    content = "";
}

Path::Path(const string& name) {
    path = fs::absolute(fs::path(name));
    content = "";
}

Path::Path(const fs::path &path) {
    this->path = path;
    content = "";
}

string Path::getName() const { return fs::absolute(path).string(); }

string Path::getFilename() const { return path.filename(); }

string Path::readFile() {
    if (content.empty()) {
        if (!fs::is_regular_file(path)) {
            return "";
        }

        ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return "";
        }

        content = string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

        file.close();

        return content;
    } else {
        return content;
    }
}

bool Path::isFile() const { return fs::is_regular_file(path); }

bool Path::isDir() const { return fs::is_directory(path); }

vector<Path> Path::getDirContent() const {
    vector<Path> vec{};
    for (const auto& entry : fs::directory_iterator(path)) {
        vec.emplace_back(entry.path());
    }
    return vec;
}

Path Path::getParent() {
    path = fs::path(getName());
    return Path(path.parent_path());
}

Path Path::getCurrentDir() { return Path(fs::current_path()); }

bool Path::operator<(const Path &other) const { return getName() < other.getName(); }

bool Path::operator>(const Path &other) const { return getName() > other.getName(); }

bool Path::operator==(const Path &other) const { return getName() == other.getName(); }