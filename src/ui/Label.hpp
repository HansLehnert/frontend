#pragma once

#include "ui/Image.hpp"
#include "core/Text.hpp"

// Text label object. Render a single line of text.
class Label : public Image {
public:
    Label() : Label("", "") {}
    Label(std::string label_text, std::string instance_name);

    // The text displayed on the label. Changing it genereates a new texture
    const std::string text() const { return text_; };
    void setText(std::string text);

    // Fixes the height of the plane that draws the label
    void setLineHeight(float line_height);

private:
    void resizeToLineHeight();

    std::string text_;
    float line_height_;
};
