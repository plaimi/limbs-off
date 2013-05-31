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

InputField::InputField() :
        text_(NULL) {
        text_ = (char*) malloc(32);
    strcpy(text_, " ");
}

bool InputField::getInput(SDL_Event &event, char* c){
    if (event.type == SDL_KEYDOWN) {
        int i = strlen(c);
        switch (event.key.keysym.sym) {
        case SDLK_RETURN:
            // User is done.
            return true;
        case SDLK_ESCAPE:
            // User doesn't want to do anything.
            c[0] = '\0';
            return true;
        case SDLK_BACKSPACE:
            // ^H if there is something to ^H.
            if (i > 0) {
                while ((c[--i] & 0xc0) == 0x80);
                c[i] = '\0';
            }
            break;
        default:
            // Limit input length.
            if (i > 27)
                return false;
            // UTF-16 to UTF-8 transcoding.
            if (event.key.keysym.unicode < 0x80) {
                c[i] = event.key.keysym.unicode;
            } else if (event.key.keysym.unicode < 0x800) {
                c[i] = (event.key.keysym.unicode >> 6) | 0xC0;
                c[++i] = (event.key.keysym.unicode & 0x3F) | 0x80;
            } else if (event.key.keysym.unicode < 0xFFFF) {
                c[i] = ((event.key.keysym.unicode >> 12)) | 0xE0;
                c[++i] = ((event.key.keysym.unicode >> 6) & 0x3F) | 0x80;
                c[++i] = ((event.key.keysym.unicode) & 0x3F) | 0x80;
            }
            c[++i] = '\0';
        }
    }
    // User is not done yet.
    return false;
}

char* InputField::getText() {
    return text_;
}

void InputField::setText(char* text) {
    text_ = text;
}


MassIndicator::MassIndicator(int position) {
    setPosition(position);
}
