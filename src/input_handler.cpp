/*
 * Copyright (C) 2011 Stian Ellingsen <stiell@stiell.org>
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

#include "input_handler.h"

InputHandler* InputHandler::handlers[SDLK_LAST];

void InputHandler::handle(SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            printf("%d/n", bindings[event.key.keysym.sym]);
    }
}

void InputHandler::bindKey(SDLKey key, ActionType action) {
    bindings[key] = action;
    handlers[key] = this;
}

InputHandler* InputHandler::getHandler(SDLKey key) {
    return handlers[key];
}
