#include <poppler-document.h>
#include <poppler-page.h>
#include <iostream>
#include <memory>
#include "Paragraph.h"


int main(int argc, char *argv[]) {

    if(argc<1) {
        return 0;
    }
    std::cerr << "file: " << argv[1] << std::endl;

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

    for (int pageIndex = 0; pageIndex < pdfDoc->pages(); ++pageIndex) {
        std::unique_ptr<poppler::page> page(pdfDoc->create_page(pageIndex));
        if (!page) {
            std::cerr << "Failed to process page " << pageIndex + 1 << std::endl;
            continue;
        }

        std::cout << "Page " << pageIndex + 1 << ":" << std::endl;

        // Extract text blocks (Paragraphs) with their bounding boxes
        auto textBlocks = page->text_list();

        std::vector<Paragraph> paragraphs;

        for (const auto& textItem : textBlocks) {

            poppler::byte_array bytetext = textItem.text().to_utf8();

            std::string word(bytetext.begin(), bytetext.end());

            auto bbox = textItem.bbox(); // Bounding box

            // std::cout << "Bounding Box: (" 
            //           << bbox.left() << ", " 
            //           << bbox.top() << ", " 
            //           << bbox.right() << ", " 
            //           << bbox.bottom() << ")" << std::endl;
            // std::cout << "word: " << word << std::endl;
            // std::cout << "----------------------------------------" << std::endl;

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
            std::cout << "New Paragraph: " << par->toJsonString() << std::endl << std::endl;
            par = nullptr; // New paragraph after new page!
        }
    }

    return 0;
}
