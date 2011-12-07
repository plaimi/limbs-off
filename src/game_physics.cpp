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

SmallBody::SmallBody(state2p s, phys_t mass, phys_t orientation, phys_t av,
        phys_t moi, Shape<phys_t>* shape) :
    Body(s, mass, orientation, av, moi, shape) {
}

void SmallBody::setDeltaState(int i, vector2p a) {
    ds_[i].p = a;
    ds_[i].v(0.0, 0.0);
}

bodystate SmallBody::getNextState(phys_t dt) {
    bodystate r;
    r.l = s_ + (ds_[0] + (ds_[1] + ds_[2]) * 2.0 + ds_[3]) * (dt / 6.0);
    r.o = remainder<phys_t> (orientation_ + dt * av_, 2 * PI);
    r.av = av_;
    return r;
}

AstroBody::AstroBody(phys_t gm, phys_t moi, phys_t av, Shape<phys_t>* shape) :
    Body(state2p()(0.0, 0.0, 0.0, 0.0), gm / G, 0.0, av, moi, shape), gm(gm) {
}

Character::Character(phys_t mass, state2p state, phys_t moi, phys_t o,
        phys_t av, Shape<phys_t>* shape) :
    SmallBody(state, mass, o, av, moi, shape) {
}

GameUniverse::GameUniverse(AstroBody* planet) :
    planet_(planet) {
}

void GameUniverse::update(phys_t dt) {
    planet_->orientation_ = remainder<phys_t> (
            planet_->orientation_ + dt * planet_->av_, 2 * PI);
    const phys_t dts[] = { 0.5 * dt, 0.5 * dt, dt };
    std::vector<SmallBody*>::iterator ib;
    for (ib = smallBodies_.begin(); ib < smallBodies_.end(); ib++) {
        SmallBody* b = *ib;
        for (int i = 0; i < 4; i++) {
            vector2p v = b->v_;
            // Calculate the distance vector to the planet.
            vector2p d = planet_->p_ - b->p_;
            if (i > 0) {
                v += b->ds_[i - 1].v * dts[i - 1];
                d += (planet_->p_ - b->ds_[i - 1].p) * dts[i - 1];
            }
            // Set initial values for the delta state.
            b->setDeltaState(i, v);
            // Calculate acceleration and update delta velocity.
            phys_t dd = d.squared();
            b->ds_[i].v += d * (planet_->gm / (sqrt<phys_t> (dd) * dd));
        }
        bodystate np = { { planet_->s_ }, { state1p()(planet_->orientation_,
                planet_->av_) } };
        bodystate bs = b->getNextState(dt);
        phys_t t;
        vector2p p, n;
        if (collide(planet_, b, np, bs, t, p, n)) {
            b->s_ = bs.l;
            vector2p pg = p + planet_->p_, pc = pg - b->p_;
            vector2p im = bounce1(b, planet_, pc, p, -n, 0.8, 0.2, 0.02);
            b->applyImpulseAt(im, pc);
            // TODO: Recalculate gravity.
            b->p_ += b->v_ * (dt * (1 - t));
        } else
            b->s_ = bs.l;
        b->orientation_ += dt * b->av_;
    }
}

void GameUniverse::addBody(SmallBody* b) {
    smallBodies_.push_back(b);
}
