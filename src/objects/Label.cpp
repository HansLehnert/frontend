#include "objects/Label.h"

#include "core/Text.h"

Label::Label(std::string label_text, std::string instance_name) :
    Image("", instance_name)
{
    text = label_text;
}


void Label::setText(std::string new_text) {
    text = new_text;

    texture = Text(new_text);
}


std::string Label::getText() {
    return text;
}


void Label::setFontSize(float size) {
    setScale((float)texture.getWidth() / texture.getHeight() * size, size);
}
