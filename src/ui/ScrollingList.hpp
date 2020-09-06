// Display a scrolling list of text labels
#pragma once

#include "ui/Frame.hpp"
#include "ui/Label.hpp"


class ScrollingList : public Frame {
public:
    ScrollingList(std::string instance_name);

    const std::vector<std::string>& items() { return items_; }
    void setItems(std::vector<std::string>& items);
    void addItem(std::string item);

    void setSize(glm::vec2 size) override;
    void setLabelHeight(float label_height);

    std::pair<int32_t, std::string> selectedItem() const { return {selected_item_, items_[selected_item_]}; }
    void selectIndex(int32_t index);
    void selectNext() { selectIndex(selected_item_ + 1); };
    void selectPrevious() { selectIndex(selected_item_ - 1); };

    float scroll_margin_;

protected:
    void updateSelf() override;

private:
    std::vector<std::string> items_;
    int32_t selected_item_;

    float scroll_position_;
    float label_height_;

    Frame cursor_;
    std::vector<Label> labels_;
};
