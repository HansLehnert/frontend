#include "ui/ScrollingList.hpp"

#include <cmath>


ScrollingList::ScrollingList(std::string instance_name) :
    Frame(instance_name),
    selected_item_(0),
    target_position_(0),
    label_height_(0.1),
    label_margin_(0),
    scroll_margin_(0),
    cursor_animation_(0)
{
    visible_ = false;

    for (Frame& frame : cursor_) {
        frame.origin_ = Plane::Origin::CENTER;
        frame.fill_color_ = glm::vec4(0, 0.8, 1, 0);
        addChild(frame);
    }

    scroll_position_ = label_height_ / 2;
    setSize(glm::vec2(1));
}


void ScrollingList::setItems(const std::vector<std::string>& items) {
    items_ = items;

    if (selected_item_ >= items_.size())
        selectIndex(-1);
}


void ScrollingList::addItem(const std::string& item) {
    items_.push_back(item);

    if (labels_.size() >= items_.size()) {
        labels_[items_.size() - 1].setText(item);
    }
}


void ScrollingList::setSize(glm::vec2 size) {
    Frame::setSize(size);
    layoutItems();
}


void ScrollingList::layoutItems() {
    label_spacing_ = label_margin_ + label_height_;
    uint32_t n_labels = static_cast<int32_t>(std::ceil(size().y / label_spacing_)) + 1;

    labels_.resize(n_labels);

    for (uint32_t i = 0; i < n_labels; i++) {
        Label& label = labels_[i];
        label.origin_ = Plane::Origin::CENTER;
        label.setLineHeight(label_height_);
        label.position_.x = size().x / 2;
        label.position_.y = label_spacing_ * i;
        addChild(label);

        if (i < items_.size()) {
            label.setText(items_[i]);
        }
    }

    for (Frame& frame : cursor_)
        frame.position_.x = size().x / 2;
}


void ScrollingList::setLabelHeight(double label_height) {
    label_height_ = label_height;
    layoutItems();
}


void ScrollingList::setLabelMargin(double label_margin) {
    label_margin_ = label_margin;
    layoutItems();
}


void ScrollingList::setScrollMargin(double scroll_margin) {
    scroll_margin_ = scroll_margin;
}


void ScrollingList::selectIndex(int32_t index) {
    const int32_t item_count = static_cast<int32_t>(items_.size());
    selected_item_ = (index % item_count + item_count) % item_count;

    const double total_margin = label_height_ / 2 + scroll_margin_;
    const double label_position = label_spacing_ * selected_item_;

    // Only scroll if new selection is out of margin bounds
    if (label_position < scroll_position_ + total_margin)
        target_position_ = label_position - total_margin;
    else if (label_position > scroll_position_ + size().y - total_margin)
        target_position_ = label_position - size().y + total_margin;
    else
        target_position_ = scroll_position_;

    // Don't scroll beyond first/last item
    target_position_ = std::max(target_position_, -label_height_ / 2);
    target_position_ = std::min(target_position_, label_spacing_ * (items_.size() - 1) - label_height_ / 2 - size().y);

    cursor_animation_ = 0;
}


void ScrollingList::updateSelf() {
    scroll_position_ += (target_position_ - scroll_position_) * .1;

    uint32_t first_item = static_cast<uint32_t>(scroll_position_ / label_spacing_);
    if (labels_[0].text() != items_[first_item]) {
        for (size_t i = 0; i < labels_.size(); i++) {
            if (i + first_item >= items_.size()) {
                labels_[i].visible_ = false;
                break;
            }

            labels_[i].setText(items_[i + first_item]);
            // TODO: swap textures instead of regenerating them all
        }
    }


    for (size_t i = 0; i < labels_.size(); i++) {
        labels_[i].position_.y = label_spacing_ * i + fmod(-scroll_position_, label_spacing_);
    }

    cursor_animation_ += cursor_animation_ < 1. ? 0.04 : -1. / cursor_.size();
    for (size_t i = 0; i < cursor_.size(); i++) {
        Frame& frame = cursor_[i];
        float frame_animation = (cursor_animation_ - i * 0.5);
        frame.position_.y = label_spacing_ * selected_item_ - scroll_position_;
        frame.setSize(glm::vec2(size().x, label_height_) + glm::vec2((frame_animation - 0.2) * label_height_));
        frame.border_color_.a = 1. - std::abs(2 * frame_animation - 1);
        frame.fill_color_.a = frame.border_color_.a * 0.2;
    }
}
