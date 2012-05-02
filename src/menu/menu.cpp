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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <GL/gl.h>
#include <SDL/SDL_ttf.h>
#include "event_code.h"
#include "menu.h"

Menu::Menu() {
    menus_[MAINMENU] = new Submenu();
    Submenu** mainmenu = menus_ + MAINMENU;
    (*mainmenu)->addButton("NEW GAME", 1, true);
    (*mainmenu)->addButton("EXIT GAME", 2, false);
    activeMenu_ = 0;
    activeElement_ = (*mainmenu)->buttons[0];
#if VERBOSE
    printf("use arrow keys to navigate the menu.\n"
            "make your choice with return or space.\n\n");
#endif
}

Menu::~Menu() {
    for (int i = 0; i < NUM_MENU; ++i) {
        for (std::vector<ScreenElement*>::const_iterator j =
                menus_[i]->buttons.begin(); j != menus_[i]->buttons.end(); ++j)
            delete (*j);
        delete menus_[i];
    }
}

int Menu::getActiveMenu() {
    return activeMenu_;
}

bool Menu::handle(const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        Uint8 *keystate = SDL_GetKeyState(NULL);
        // Leave left alt alone
        if (keystate[SDLK_LALT])
            return false;
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
            // New game
            if (activeElement_->getPosition() == 1)
                raiseEvent(NEW_GAME_EVENT);
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
    case NEW_GAME_EVENT:
        event.type = SDL_USEREVENT;
        event.user.code = NEW_GAME;
        break;
    case QUIT_EVENT:
        event.type = SDL_QUIT;
    }
    SDL_PushEvent(&event);
}

void Menu::setSelected(ScreenElement* selected) {
    activeElement_->setSelected(false);
    activeElement_ = selected;
    activeElement_->setSelected(true);
}
