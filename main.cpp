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

            // std::string word(bytetext.begin(), bytetext.end());
            std::string word(bytetext.data(), bytetext.size());

            // // Convert poppler::byte_array to std::wstring
            // poppler::byte_array wbytetext = textItem.text().to_utf16();
            // std::wstring wstringData(reinterpret_cast<const wchar_t*>(wbytetext.data()), wbytetext.size() / sizeof(wchar_t));
            // // Print the wstring content
            // std::wcout << L"Wide string content: " << wstringData << std::endl;

            auto bbox = textItem.bbox(); // Bounding box

            if( !par || !par->belongs(textItem) ) {
                if(par) {
                    json j = *par;
                    std::cout << j.dump(4) << std::flush;

                    // ack
                    std::string acknowledgment;
                    std::getline(std::cin, acknowledgment);
                    if (acknowledgment != "ACK") {
                        std::cerr << "No acknowledgment received!" << std::endl;
                        return 1;
                    }

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
        
        // all_paragraphs.insert(all_paragraphs.end(), paragraphs.begin(), paragraphs.end());
        // json j = all_paragraphs;
        // std::cout << j.dump(4);
    }

    return 0;
}
