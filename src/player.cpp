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

#include <SDL/SDL.h>
#include "character.h"
#include "player.h"

Player::Player(Character* character) :
    Actor(character) {
        // Initialise bindings_ to avoid gibberish
        for (int i = 0; i < SDLK_LAST; ++i) {
            bindings_[i] = NOTHING;
        }
}

bool Player::handle(SDL_Event event) {
    // Check that event is a keyboard event
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) {
        return false;
    }
    // Check that there is a binding for the key
    if (!bindings_[event.key.keysym.sym]) {
        return false;
    }
    if (event.type == SDL_KEYDOWN) {
        switch (bindings_[event.key.keysym.sym]) {
        case LEFT:
            character_->move(-1.0);
            break;
        case RIGHT:
            character_->move(1.0);
            break;
        case JUMP:
            character_->jump(true);
            break;
        case CROUCH:
            character_->crouch(true);
            break;
        case FIRE:
            character_->fire(true);
            break;
        case LPUNCH:
            character_->leftPunch(true);
            break;
        case RPUNCH:
            character_->rightPunch(true);
            break;
        case LKICK:
            character_->leftKick(true);
            break;
        case RKICK:
            character_->rightKick(true);
            break;
        default:
            return false;
        }
    }
    if (event.type == SDL_KEYUP) {
        switch (bindings_[event.key.keysym.sym]) {
        case LEFT:
            character_->move(fmax(character_->getVel(), 0));
            break;
        case RIGHT:
            character_->move(fmin(character_->getVel(), 0));
            break;
        case JUMP:
            character_->jump(false);
            break;
        case CROUCH:
            character_->crouch(false);
            break;
        case FIRE:
            character_->fire(false);
            break;
        case LPUNCH:
            character_->leftPunch(false);
            break;
        case RPUNCH:
            character_->rightPunch(false);
            break;
        case LKICK:
            character_->leftKick(false);
            break;
        case RKICK:
            character_->rightKick(false);
            break;
        default:
            return false;
        }
    }
    return true;
}

void Player::bindKey(SDLKey key, ActionType action) {
    bindings_[key] = action;
}
