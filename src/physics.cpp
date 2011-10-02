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

#include "physics.h"

Particle::~Particle() {
}

state2p Particle::getState() {
    return s_;
}

vector2p Particle::getPosition() {
    return p_;
}

void Particle::getPosition(phys_t& x, phys_t& y) {
    x = x_;
    y = y_;
}

vector2p Particle::getVelocity() {
    return v_;
}

void Particle::getVelocity(phys_t& vx, phys_t& vy) {
    vx = vx_;
    vy = vy_;
}

Mass::~Mass() {
}

phys_t Mass::getMass() {
    return mass_;
}

phys_t Body::getOrientation() {
    return orientation_;
}

phys_t Body::getAngularVelocity() {
    return av_;
}

Body::~Body() {
}
