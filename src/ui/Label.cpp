#include "ui/Label.hpp"


Label::Label(std::string label_text, std::string instance_name) :
        Image(nullptr, instance_name)
{
    if (instance_name == "") {
        instance_name = "label_" + label_text;
    }

    setText(label_text);
}


void Label::setText(std::string text) {
    this->text = text;

    texture = renderText(text);
    scale.x = scale.y / texture->getHeight() * texture->getWidth();
}


std::string Label::getText() {
    return text;
}


void Label::setLineHeight(float line_height) {
    scale.y = line_height;
    scale.x = line_height / texture->getHeight() * texture->getWidth();
}
