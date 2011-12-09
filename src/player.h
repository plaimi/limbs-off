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

#ifndef PLAYER_H_
#define PLAYER_H_ 

#include "actor.h"
#include "character.h"
#include "event_handler.h"

class Player: public Actor, public EventHandler {
public:
    Player(Character* character);
    // Handle keyboard event
    bool handle(SDL_Event event);
    // Bind key to action
    void bindKey(SDLKey key, ActionType action);
private:
    // All bindindgs
    ActionType bindings_[SDLK_LAST];
};

#endif /* PLAYER_H_ */
