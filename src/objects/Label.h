#pragma once

#include "ui/Image.hpp"
#include "core/Text.hpp"

/**
 * @brief Text label object
 *
 * Used for rendering text.
 */
class Label : public Image {
public:
    /**
     * @brief Construct a new Label object
     */
    Label(std::string label_text = "", std::string instance_name = "");

    /**
     * @brief Set the text value of the label
     */
    void setText(std::string new_text);

    /**
     * @brief Get the text value
     */
    std::string getText();

    /**
     * @brief Set the vertical size of the label
     */
    void setFontSize(float size);

    TextFormat style;

private:
    std::string text;
};
