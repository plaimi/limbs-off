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

#include <vector>
#include "physics.h"

class Universe;

class SmallBody: public Body {
protected:
    SmallBody(state2p s, phys_t mass, phys_t orientation, phys_t av,
            phys_t moi, Shape<phys_t>* shape);
private:
    void setDeltaState(int i, vector2p a);
    bodystate getNextState(phys_t dt);
    state2p ds_[4];
    friend class GameUniverse;
};

class AstroBody: public Body {
public:
    const phys_t gm;
    AstroBody(phys_t gm, phys_t moi, phys_t av, Shape<phys_t>* shape);
    friend class GameUniverse;
};

class Character: public SmallBody {
public:
    Character(phys_t mass, state2p state, phys_t moi, phys_t o, phys_t av,
            Shape<phys_t>* shape);
};

class GameUniverse: public Universe {
public:
    GameUniverse(AstroBody* planet);
    void update(phys_t dt);
    void addBody(SmallBody* b);
private:
    AstroBody* planet_;
    std::vector<SmallBody*> smallBodies_;
};

#endif /* GAME_PHYSICS_H_ */
