#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include <poppler-page.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class BBox {
    double m_x;
    double m_y;
    double m_width;
    double m_height;

public:
    BBox(const poppler::rectf &bbox) : m_x(bbox.x()), m_y(bbox.y()), m_width(bbox.width()), m_height(bbox.height()) { }
    // Define how to serialize Outer to JSON
    friend void to_json(json& j, const BBox& b);
};

class Paragraph {
    int m_page;
    poppler::rectf m_bbox; // rectangle< double >
    std::vector<std::string> m_words;
    bool eos() const;

public:
    Paragraph();
    Paragraph(const int page, const poppler::rectf &bbox);
    void append(const std::string &word);

    bool belongs(const poppler::text_box &textItem) const;
    void expand(const poppler::text_box &textItem);
    
    std::string wordsToString() const;
    std::string toJsonString() const;

    // Define how to serialize Outer to JSON
    friend void to_json(json& j, const Paragraph& p);
};


#endif