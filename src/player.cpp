/*
 * Copyright (C) 2011 Alexander Berntsen <alexanderberntsen@plaimi.net>
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
    for (int i = 0; i < SDLK_LAST; ++i)
        keyBindings_[i] = NOTHING;
    for (int i = 0; i < 256; ++i)  {
        joyAxisBindings_[i] = NOTHING;
        joyButtonBindings_[i] = NOTHING;
    }
}

bool Player::handle(const SDL_Event& event) {
    double value;
    int input;
    ActionType* bindings;
    switch (event.type) {
    case SDL_KEYUP:
    case SDL_KEYDOWN:
        value = event.key.state == SDL_PRESSED ? 1.0 : 0.0;
        input = event.key.keysym.sym;
        bindings = keyBindings_;
        break;
    case SDL_JOYBUTTONUP:
    case SDL_JOYBUTTONDOWN:
        value = event.jbutton.state == SDL_PRESSED ? 1.0 : 0.0;
        input = event.jbutton.button;
        bindings = joyButtonBindings_;
        break;
    case SDL_JOYAXISMOTION:
        value = event.jaxis.value / 32768.0;
        input = event.jaxis.which;
        bindings = joyAxisBindings_;
        break;
    default:
        return false;
    }
    switch (bindings[input]) {
    case LEFT:
            character_->moveLeft(value);
        break;
    case RIGHT:
            character_->moveRight(value);
        break;
    case JUMP:
        character_->jump(value);
        break;
    case CROUCH:
        character_->crouch(value);
        break;
    case FIRE:
        character_->fire(value);
        break;
    case LPUNCH:
        character_->leftPunch(value);
        break;
    case RPUNCH:
        character_->rightPunch(value);
        break;
    case LKICK:
        character_->leftKick(value);
        break;
    case RKICK:
        character_->rightKick(value);
        break;
    default:
        return false;
    }
    return true;
}

void Player::bindKey(SDLKey key, ActionType action) {
    keyBindings_[key] = action;
}

void Player::bindJoyButton(Uint8 joyButton, ActionType action) {
    joyButtonBindings_[joyButton] = action;
}
