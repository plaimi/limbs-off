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

void Particle::getPosition(float& x, float& y) {
    x = x_;
    y = y_;
}

void Particle::getVelocity(float& vx, float& vy) {
    vx = vx_;
    vy = vy_;
}

Mass::~Mass() {
}

float Mass::getMass() {
    return mass_;
}

float Body::getOrientation() {
    return orientation_;
}

float Body::getAngularVelocity() {
    return av_;
}

Body::~Body() {
}
