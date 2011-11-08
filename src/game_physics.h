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

#ifndef GAME_PHYSICS_H_
#define GAME_PHYSICS_H_

#include "physics.h"

class Universe;

class SmallBody: public Body {
protected:
    SmallBody(state2p s, phys_t mass, phys_t orientation, phys_t av);
private:
    void update(phys_t dt);
    void setDeltaState(int i, vector2p a);
    state2p ds_[4];
    friend class Universe;
};

class AstroBody: public Body {
public:
    const phys_t gm;
    AstroBody(phys_t gm, phys_t av);
    friend class Universe;
};

class Character: public SmallBody {
public:
    Character(phys_t mass, state2p state, phys_t o, phys_t av);
};

class Universe {
public:
    Universe(AstroBody* planet, Character* character);
    void update(phys_t dt);
private:
    AstroBody* planet_;
    Character* character_;
};

#endif /* GAME_PHYSICS_H_ */
