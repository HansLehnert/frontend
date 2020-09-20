// ScrollingList class
//
// Displays a scrolling list that displays text labels along with a cursor.
// Only displays text in order to optimize for large amount of items.

#pragma once

#include "ui/Frame.hpp"
#include "ui/Label.hpp"


class ScrollingList : public Frame {
public:
    ScrollingList(std::string instance_name);

    const std::vector<std::string>& items() { return items_; }
    void setItems(const std::vector<std::string>& items);
    void addItem(const std::string& item);

    void setSize(glm::vec2 size) override;
    void setLabelHeight(double label_height);

    // Set the space between the each label
    void setLabelMargin(double label_margin);

    // Set the margin for the cursor to the start/end of the list
    void setScrollMargin(double scroll_margin);

    std::pair<int32_t, std::string> selectedItem() const { return {selected_item_, items_[selected_item_]}; }
    void selectIndex(int32_t index);
    void selectNext() { selectIndex(selected_item_ + 1); };
    void selectPrevious() { selectIndex(selected_item_ - 1); };


protected:
    void updateSelf() override;

private:
    void layoutItems();

private:
    std::vector<std::string> items_;
    uint32_t selected_item_;

    double scroll_position_;  // current scroll position
    double target_position_;
    double label_height_;
    double label_margin_;
    double label_spacing_;
    double scroll_margin_;
    double cursor_animation_;

    std::array<Frame, 2> cursor_;
    std::vector<Label> labels_;
};
