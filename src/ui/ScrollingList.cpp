#include "ui/ScrollingList.hpp"

#include <cmath>


ScrollingList::ScrollingList(std::string instance_name) :
    Frame(instance_name),
    selected_item_(0),
    label_height_(0.1)
{
    cursor_.fill_color_ = glm::vec4(1, 0, 0, 0.2);
    addChild(cursor_);

    setSize(glm::vec2(1));
}


void ScrollingList::setItems(std::vector<std::string>& items) {
    items_ = items;
}


void ScrollingList::addItem(std::string item) {
    items_.push_back(item);

    if (labels_.size() >= items_.size()) {
        labels_[items_.size() - 1].setText(item);
    }
}


void ScrollingList::setSize(glm::vec2 size) {
    Frame::setSize(size);

    cursor_.setSize(glm::vec2(size.x, label_height_));

    uint32_t n_labels = static_cast<int32_t>(std::ceil(size.y / label_height_)) + 1;

    labels_.resize(n_labels + 1);

    for (uint32_t i = 0; i < n_labels; i++) {
        Label& label = labels_[i];
        label.setLineHeight(label_height_);
        label.position_.x = 0;
        label.position_.y = label_height_ * i;
        addChild(label);

        if (i < items_.size()) {
            label.visible_ = true;
            label.setText(items_[i]);
        }
        else
            label.visible_ = false;
    }
}


void ScrollingList::selectIndex(int32_t index) {
    selected_item_ = index % static_cast<int32_t>(items_.size());
    cursor_.position_.y = labels_[selected_item_].position_.y;
}


void ScrollingList::setLabelHeight(float label_height) {
    label_height_ = label_height;
    setSize(size());
}


void ScrollingList::updateSelf() {

}
