#include "Paragraph.h"

Paragraph::Paragraph() : m_page(0) { }

Paragraph::Paragraph(const int page, const poppler::rectf &bbox) : m_page(page), m_bbox(bbox) { }

void Paragraph::append(const std::string &word) { m_words.push_back(word); }

bool Paragraph::eos() const
{
    if(m_words.size() && m_words.back().back() == '.') {
        return true;
    }
    return false;
}

bool Paragraph::belongs(const poppler::text_box &textItem) const
{
    const poppler::rectf &bbox = textItem.bbox();

    if(m_bbox.bottom() == bbox.bottom()) {
        return true;
    } else { // Different line
        if(eos()) { // last character was EOS => new paragraph
            return false;
        } else {
            return true;
        }
    }
}

void Paragraph::expand(const poppler::text_box &textItem)
{
    const poppler::rectf &bbox = textItem.bbox();

    if(m_bbox.bottom() < bbox.bottom()) {
        m_bbox.set_bottom(bbox.bottom());
    }
    if(m_bbox.top() > bbox.top()) {
        m_bbox.set_top(bbox.top());
    }
    if(m_bbox.right() < bbox.right()) {
        m_bbox.set_right(bbox.right());
    }
    if(m_bbox.left() > bbox.left()) {
        m_bbox.set_left(bbox.left());
    }
}

std::string Paragraph::wordsToString() const {

    std::string par;

    for(const auto word: m_words) {
        par += word + ' ';
    }
    return par;
}

void to_json(json& j, const Paragraph& p) {
    j = json{ {"page", p.m_page}, {"text", p.wordsToString()}, {"bbox", BBox(p.m_bbox)} };
}

void to_json(json& j, const BBox& b)
{
    j = json{ {"x", b.m_x}, {"y", b.m_y}, {"width", b.m_width}, {"height", b.m_height} };
}

std::string Paragraph::toJsonString() const 
{
    json j = *this;
    return j.dump(4);
}
