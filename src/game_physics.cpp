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

#include <math.h>
#include "geometry.h"
#include "game_physics.h"

SmallBody::SmallBody(state2p s, phys_t mass, phys_t orientation, phys_t av) :
    Body(s, mass, orientation, av) {
}

void SmallBody::update(phys_t dt) {
    s_ += (ds_[0] + (ds_[1] + ds_[2]) * 2.0 + ds_[3]) * (dt / 6.0);
    orientation_ = remainderp(orientation_ + dt * av_, 2 * PI);
}

void SmallBody::setDeltaState(int i, vector2p a) {
    ds_[i].p = a;
    ds_[i].v(0.0, 0.0);
}

AstroBody::AstroBody(phys_t gm, phys_t av) :
    Body(state2p()(0.0, 0.0, 0.0, 0.0), gm / G, 0.0, av), gm(gm) {
}

Character::Character(phys_t mass, state2p state, phys_t o, phys_t av) :
    SmallBody(state, mass, o, av) {
}

Universe::Universe(AstroBody* planet, Character* character) :
    planet_(planet), character_(character) {
}

void Universe::update(phys_t dt) {
    planet_->orientation_ = remainderp(
            planet_->orientation_ + dt * planet_->av_, 2 * PI);
    const phys_t dts[] = { 0.5 * dt, 0.5 * dt, dt };
    for (int i = 0; i < 4; i++) {
        vector2p v = character_->v_;
        // Calculate the distance vector to the planet.
        vector2p d = planet_->p_ - character_->p_;
        if (i > 0) {
            v += character_->ds_[i - 1].v * dts[i - 1];
            d += (planet_->p_ - character_->ds_[i - 1].p) * dts[i - 1];
        }
        // Set initial values for the delta state.
        character_->setDeltaState(i, v);
        // Calculate acceleration and update delta velocity.
        phys_t dd = d.squared();
        character_->ds_[i].v += d * (planet_->gm / (sqrtp(dd) * dd));
    }
    character_->update(dt);
}
