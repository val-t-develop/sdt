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
 *  Wrapper for vector of pages.
 */
#include "Pages.hpp"

void Pages::addPage(int render) {
    if (render) {
        pages.push_back(&document->GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4)));
    }
}

PdfPage *Pages::getActivePage(int render) {
    if (render) {
        if ((active + 1) * render > pages.size()) {
            addPage(render);
        }
    }
    return pages[active * render];
}

void Pages::removeActivePage(int render) {
    if (render) {
        pages.erase(pages.begin() + active);
        document->GetPages().RemovePageAt(active);
        active--;
    }
}