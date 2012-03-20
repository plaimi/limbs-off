/*
 * Copyright (C) 2012 Stian Ellingsen <stiell@stiell.org>
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

#include <GL/gl.h>
#include <SDL/SDL_ttf.h>
#include "menu.h"

Menu::Menu() {
    menus_[MAINMENU] = new Submenu();
    Submenu** mainmenu = menus_ + MAINMENU;
    (*mainmenu)->addButton("RESUME", 1, true);
    (*mainmenu)->addButton("EXIT GAME", 2, false);
    activeMenu_ = 0;
    activeElement_ = (*mainmenu)->buttons[0];
}

int Menu::getActiveMenu() {
    return activeMenu_;
}

bool Menu::handle(const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_DOWN:
            setSelected(menus_[activeMenu_]->buttons[std::min(
                        activeElement_->getPosition(), 
                        (int) menus_[activeMenu_]->buttons.size() - 1)]);
            break;
        case SDLK_UP:
            setSelected(menus_[activeMenu_]->buttons[std::max(
                        activeElement_->getPosition() - 2, 0)]);
            break;
        case SDLK_SPACE:
            ; // Fall through
        case SDLK_RETURN:
            // Resume game
            if (activeElement_->getPosition() == 1)
                ;
            // Exit game
            else {
                raiseEvent(QUIT_EVENT);
            }
            return true;
        }
    }
    return false;
}

Submenu* Menu::getMenu(int menu) {
    return menus_[menu];
}

void Menu::raiseEvent(EVENT_ID id) {
    SDL_Event event;
    switch(id) {
    case QUIT_EVENT:
        event.type = SDL_QUIT;
    }
    SDL_PushEvent(&event);
}

void Menu::setSelected(GuiElement* selected) {
    activeElement_->setSelected(false);
    activeElement_ = selected;
    activeElement_->setSelected(true);
}
