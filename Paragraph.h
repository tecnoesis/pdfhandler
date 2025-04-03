#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include <poppler-page.h>
#include <vector>
#include <string>

class Paragraph {
    int m_page;
    poppler::rectf m_bbox; // rectangle< double >
    std::vector<std::string> m_words;
    poppler::rectf m_last_bbox;
    bool eos() const;

public:
    Paragraph();
    Paragraph(const int page, const poppler::rectf &bbox);
    void append(const std::string &word);

    bool belongs(const poppler::text_box &textItem) const;
    void expand(const poppler::text_box &textItem);
    std::string toString() const;
};


#endif