/*
 * Copyright (C) 2012, 2013 Alexander Berntsen <alexander@plaimi.net>
 *
 * This file is part of Limbs Off.
 *
 * Limbs Off is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Limbs Off is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Limbs Off.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "screen_element.hxx"
#include <stdlib.h>
#include <string.h>

ScreenElement::ScreenElement() :
        position_(0) {
}

int ScreenElement::getPosition() {
    return position_;
}

void ScreenElement::setPosition(int pos) {
    position_ = pos;
}

GuiElement::GuiElement() :
        selected_(false) {
}

bool GuiElement::isSelected() {
    return selected_;
}

void GuiElement::setSelected(bool selected) {
    selected_ = selected;
}

Button::Button(const char* text, int position, bool selected) :
        text_(NULL) {
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

MassIndicator::MassIndicator(int position) {
    setPosition(position);
}
