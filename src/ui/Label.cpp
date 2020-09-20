#include "ui/Label.hpp"


Label::Label(std::string text, std::string instance_name) :
        Image(nullptr, instance_name),
        line_height_(0.1)
{
    fill_mode_ = Image::FillMode::FIT;
    setText(text);
}


void Label::setText(std::string text) {
    if (text == text_)
        return;

    text_ = text;

    setTexture(renderText(text));
    setLineHeight(line_height_);  // Resize the label to keep current line height
}


void Label::setLineHeight(float line_height) {
    line_height_ = line_height;
    resizeToLineHeight();
}


void Label::resizeToLineHeight() {
    setSize(glm::vec2(line_height_ * texture().aspectRatio(), line_height_));
}
