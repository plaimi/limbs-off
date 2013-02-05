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

#ifndef SCREEN_ELEMENT_HXX_
#define SCREEN_ELEMENT_HXX_

class ScreenElement {
public:
    ScreenElement();
    virtual ~ScreenElement() { }
    int getPosition();
    void setPosition(int pos);
protected:
    int position_;
};

class GuiElement: public ScreenElement {
public:
    GuiElement();
    bool isSelected();
    void setSelected(bool selected);
protected:
    bool selected_;
};

class Button: public GuiElement {
public:
    Button(const char* text, int position, bool selected = false);
    virtual ~Button();
    char* getText();
private:
    char* text_;
};

class HudElement: public ScreenElement {
};

class MassIndicator: public HudElement {
public:
    MassIndicator(int position);
};

#endif /*SCREEN_ELEMENT_HXX_*/
