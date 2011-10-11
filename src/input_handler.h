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

#ifndef INPUT_HANDLER_H_
#define INPUT_HANDLER_H_

#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include "action.h"

class InputHandler {
public:
    /**
     * Return the input handler.
     */
    static InputHandler* getHandler(SDLKey key);
    /**
     * Treat input event.
     */
    void handle(SDL_Event event);
    /**
     * Pair a key with an action type.
     */
    void bindKey(SDLKey key, ActionType action);
private:
    static InputHandler* handlers[SDLK_LAST];
    ActionType bindings[SDLK_LAST];
};

#endif /* INPUT_HANDLER_H_ */
