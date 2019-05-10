#pragma once

#include "ui/Image.hpp"
#include "core/Text.hpp"

// Text label object. Render a single line of text.
class Label : public Image {
public:
    /**
     * @brief Construct a new Label object
     */
    Label(std::string label_text = "", std::string instance_name = "");

    /**
     * @brief Set the text value of the label
     */
    void setText(std::string text);

    /**
     * @brief Get the text value
     */
    std::string getText();

    /**
     * @brief Set the vertical size of the label
     */
    void setLineHeight(float line_height);

private:
    std::string text;
};
