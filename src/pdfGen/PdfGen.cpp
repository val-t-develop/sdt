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
#include <podofo/podofo.h>
#include <utils/ArgsParser.hpp>
using namespace PoDoFo;

PdfGen::PdfGen(vector<shared_ptr<Node>> _nodes) : nodes(_nodes) {}

void PdfGen::genPdf() {
    PdfMemDocument document;
    PdfPainter painter;
    PdfFont* font;

    try {
        auto& page = document.GetPages().CreatePage(PdfPage::CreateStandardPageSize(PdfPageSize::A4));
        painter.SetCanvas(page);

        font = document.GetFonts().SearchFont("Arial");
        if (font == nullptr)
            throw std::runtime_error("Invalid handle");

        auto& metrics = font->GetMetrics();
        cout << "The font name is "<< metrics.GetFontName() << endl;
        cout << "The family font name is " << metrics.GetFontFamilyName() << endl;
        cout << "The font file path is " << metrics.GetFilePath() << endl;
        cout << "The font face index is " << metrics.GetFaceIndex() << endl;

        painter.TextState.SetFont(*font, 18);

        painter.DrawText("ABCDEFGHIKLMNOPQRSTVXYZ", 56.69, page.GetRect().Height - 56.69);

        try {
            painter.DrawText("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЬЫЭЮЯ", 56.69, page.GetRect().Height - 80);
        }
        catch (PdfError& err) {
            if (err.GetCode() == PdfErrorCode::InvalidFontData)
                cout << "WARNING: The matched font \"" << metrics.GetFontName() << "\" doesn't support cyrillic" << endl;
        }

        painter.FinishDrawing();

        document.GetMetadata().SetCreator(PdfString("SDT - Scientific Document Tool (powered by PoDoFo)"));
        document.GetMetadata().SetAuthor(PdfString(""));
        document.GetMetadata().SetTitle(PdfString(""));
        document.GetMetadata().SetSubject(PdfString(""));
        document.GetMetadata().SetKeywords(vector<string>({ ""}));

        document.Save(ArgsParser::output->getFilename());
    }
    catch (PdfError& e){
        try{painter.FinishDrawing();} catch (...){}
        throw e;
    }
}
