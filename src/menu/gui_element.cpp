#include "menu.h"

bool GuiElement::isSelected() {
    return selected_;
}

int GuiElement::getPosition() {
    return position_;
}

void GuiElement::setPosition(int pos) {
    position_ = pos;
}

void GuiElement::setSelected(bool selected) {
    selected_ = selected;
}

Button::Button(const char* text, int position, bool selected) {
    text_ = (char*) malloc(strlen(text) + 1);
    strcpy(text_, text);
    setSelected(selected);
    setPosition(position);
}

Button::~Button() {
    free(text_);
}

char* Button::getText() {
    return text_;
}

int Button::getPosition() {
    return position_;
}
