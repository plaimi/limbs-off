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

#ifndef MENU_HXX_
#define MENU_HXX_

#include <vector>
#include "event_handler.hxx"
#include "screen_element.hxx"

class Submenu {
public:
    Submenu();
    void addButton(const char* text, bool selected);
    /** Buttons are hardcoded. */
    std::vector<Button*> buttons;
};

class Menu: public EventHandler {
public:
    Menu();
    virtual ~Menu();
    enum {
        MAINMENU,
        OPTIONS,
        NUM_MENU
    };
    enum EVENT_ID {
        QUIT_EVENT,
        NEW_GAME_EVENT
    };
    bool handle(const SDL_Event &event);
    int getActiveMenu();
    void setActiveMenu(int menu);
    Submenu* getMenu(int menu);
private:
    Menu& operator=(const Menu&);
    Menu(const Menu&);
    Button* getButton(std::vector<Button*> vector, int position);
    Button* getActiveElement();
    void setActiveElement(Button* active);
    void raiseEvent(EVENT_ID id);
    Button* activeElement_;
    int activeMenu_;
    /** Menus are hardcoded. */
    Submenu* menus_[NUM_MENU];
};

#endif /* MENU_HXX_ */
