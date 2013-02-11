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
#include "event_code.hxx"
#include "menu.hxx"

Menu::Menu() :
        // setActiveMenu calls setActiveElement which needs activeElement
        activeElement_(NULL),
        activeMenu_(0) {
    menus_[MAINMENU] = new Submenu();
    menus_[OPTIONS] = new Submenu();
    Submenu** mainmenu = menus_ + MAINMENU;
    Submenu** options = menus_ + OPTIONS;
    (*mainmenu)->addButton("NEW GAME", false);
    (*mainmenu)->addButton("OPTIONS", false);
    (*mainmenu)->addButton("EXIT GAME", false);
    (*options)->addButton("BACK TO MAIN MENU", false);
    setActiveMenu(MAINMENU);
#if VERBOSE
    printf("use arrow keys to navigate the menu.\n"
            "make your choice with return or space.\n\n");
#endif
}

Menu::~Menu() {
    for (int i = 0; i < NUM_MENU; ++i) {
        for (std::vector<Button*>::const_iterator j =
                menus_[i]->buttons.begin(); j != menus_[i]->buttons.end(); ++j)
            delete (*j);
        delete menus_[i];
    }
}

int Menu::getActiveMenu() {
    return activeMenu_;
}

void Menu::setActiveMenu(int menu) {
    if (menu >= NUM_MENU || menu < 0)
        return;
    activeMenu_ = menu;
    setActiveElement(menus_[activeMenu_]->buttons[0]);
}

bool Menu::handle(const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        Uint8 *keystate = SDL_GetKeyState(NULL);
        // Leave left alt alone
        if (keystate[SDLK_LALT])
            return false;
        switch (event.key.keysym.sym) {
        case SDLK_DOWN:
            setActiveElement(menus_[activeMenu_]->buttons[std::min(
                        activeElement_->getPosition(), 
                        (int) menus_[activeMenu_]->buttons.size() - 1)]);
            break;
        case SDLK_UP:
            setActiveElement(menus_[activeMenu_]->buttons[std::max(
                        activeElement_->getPosition() - 2, 0)]);
            break;
        case SDLK_SPACE:
            ; // Fall through
        case SDLK_RETURN:
            char* activeElement = activeElement_->getText();
            if (!strcmp(activeElement, "NEW GAME"))
                raiseEvent(NEW_GAME_EVENT);
            else if (!strcmp(activeElement, "OPTIONS"))
                setActiveMenu(OPTIONS);
            else if (!strcmp(activeElement, "EXIT GAME"))
                raiseEvent(QUIT_EVENT);
            else if (!strcmp(activeElement, "BACK TO MAIN MENU"))
                setActiveMenu(MAINMENU);
            return !strcmp(activeElement, "OPTIONS") || !strcmp(activeElement,
                    "BACK TO MAIN MENU") ? false : true;
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

void Menu::setActiveElement(Button* element) {
    if (activeElement_)
        activeElement_->setSelected(false);
    activeElement_ = element;
    activeElement_->setSelected(true);
}
