#include "objects/Label.h"

Label::Label(std::string label_text, std::string instance_name) :
        Image(nullptr, instance_name)
{
    setText(label_text);
}


void Label::setText(std::string new_text) {
    text = new_text;

    texture = renderText(text, style);
}


std::string Label::getText() {
    return text;
}


void Label::setFontSize(float size) {
    scale.x = (float)texture->getWidth() / texture->getHeight() * size;
    scale.y = size;
}
