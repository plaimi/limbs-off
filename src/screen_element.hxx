/*
 * Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
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

#ifndef SCREEN_ELEMENT_H_
#define SCREEN_ELEMENT_H_

class ScreenElement {
public:
    bool isSelected();
    int getPosition();
    void setPosition(int pos);
    void setSelected(bool selected);
protected:
    bool selected_;
    int position_;
};

class Button: public ScreenElement {
public:
    Button(const char* text, int position, bool selected = false);
    ~Button();
    char* getText();
    int getPosition();
private:
    bool selected_;
    char* text_;
    int position_;
};

#endif /*SCREEN_ELEMENT_H_*/
