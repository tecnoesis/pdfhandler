#include <poppler-document.h>
#include <poppler-page.h>
#include <iostream>
#include <memory>
#include "Paragraph.h"


int main(int argc, char *argv[]) {

    if(argc<2) {
        return 0;
    }
    // Load the PDF document
    std::string filePath = argv[1];
    std::unique_ptr<poppler::document> pdfDoc(poppler::document::load_from_file(filePath));

    if (!pdfDoc) {
        std::cerr << "Failed to open PDF file: " << filePath << std::endl;
        return 1;
    }

    // Loop through all pages in the document

    bool newpar = true;
    Paragraph *par = nullptr;

    std::vector<Paragraph> all_paragraphs;

    for (int pageIndex = 0; pageIndex < pdfDoc->pages(); ++pageIndex) {
        std::unique_ptr<poppler::page> page(pdfDoc->create_page(pageIndex));
        if (!page) {
            std::cerr << "Failed to process page " << pageIndex + 1 << std::endl;
            continue;
        }

        // Extract text blocks (Paragraphs) with their bounding boxes
        auto textBlocks = page->text_list();

        std::vector<Paragraph> paragraphs;

        for (const auto& textItem : textBlocks) {

            poppler::byte_array bytetext = textItem.text().to_utf8();

            std::string word(bytetext.begin(), bytetext.end());

            auto bbox = textItem.bbox(); // Bounding box

            if( !par || !par->belongs(textItem) ) {
                if(par) {
                    std::cout << "New Paragraph: " << par->toJsonString() << std::endl << std::endl;
                }
                paragraphs.emplace_back(Paragraph(pageIndex, bbox));
                par = &paragraphs.back();
            } else {
                par->expand(textItem);
            }
            par->append(word);
        }
        if(par) {
            par = nullptr; // New paragraph after new page!
        }
        
        all_paragraphs.insert(all_paragraphs.end(), paragraphs.begin(), paragraphs.end());
        json j = all_paragraphs;
        std::cout << j.dump(4);
    }

    return 0;
}
