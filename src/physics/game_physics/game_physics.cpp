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
#include "physics/game_physics.h"

SmallBody::SmallBody(state2p s, phys_t mass, phys_t orientation, phys_t av,
        phys_t moi, Shape<phys_t>* shape) :
    Body(s, mass, orientation, av, moi, shape) {
}

bool SmallBody::interact(AstroBody* b, double dt, vector2p& p, vector2p& im) {
    return false;
}

void SmallBody::setDeltaState(int i, vector2p a) {
    ds_[i].p = a;
    ds_[i].v(0.0, 0.0);
}

bodystate SmallBody::getNextState(phys_t dt) {
    bodystate r;
    r.l = s_ + (ds_[0] + (ds_[1] + ds_[2]) * 2.0 + ds_[3]) * (dt / 6.0);
    r.a.p = remainder<phys_t> (orientation_ + dt * av_, 2 * PI);
    r.a.v = av_;
    return r;
}

AstroBody::AstroBody(phys_t gm, phys_t moi, phys_t av, Shape<phys_t>* shape) :
    Body(state2p()(0.0, 0.0, 0.0, 0.0), gm / G, 0.0, av, moi, shape), gm(gm) {
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
            vector2p v = b->getVelocity();
            // Calculate the distance vector to the planet.
            vector2p d = planet_->getPosition() - b->getPosition();
            if (i > 0) {
                v += b->ds_[i - 1].v * dts[i - 1];
                d += (planet_->getPosition() - b->ds_[i - 1].p) * dts[i - 1];
            }
            // Set initial values for the delta state.
            b->setDeltaState(i, v);
            // Calculate acceleration and update delta velocity.
            phys_t dd = d.squared();
            b->ds_[i].v += d * (planet_->gm / (sqrt<phys_t> (dd) * dd));
        }
        bodystate np = { planet_->s_, state1p()(planet_->orientation_,
                planet_->getAngularVelocity()) };
        bodystate bs = b->getNextState(dt);
        phys_t t;
        vector2p p, n;
        if (collide(planet_, b, np, bs, t, p, n)) {
            b->s_ = bs.l;
            vector2p pg = p + planet_->getPosition(), pc = pg
                    - b->getPosition();
            vector2p im = bounce1(b, planet_, pc, p, -n, 0.8, 0.2, 0.02);
            b->applyImpulseAt(im, pc);
            // TODO: Recalculate gravity.
            b->s_.p += b->getVelocity() * (dt * (1 - t));
        } else
            b->s_ = bs.l;
        vector2p pg, im;
        if (b->interact(planet_, dt, pg, im))
            b->applyImpulseAt(im, pg - b->getPosition());
        b->orientation_ += dt * b->av_;
    }
    std::vector<Link*>::iterator il;
    for (il = links_.begin(); il < links_.end(); il++)
        (*il)->update(dt, this);
}

void GameUniverse::addBody(SmallBody* b) {
    smallBodies_.push_back(b);
}

void GameUniverse::addLink(Link* l) {
    links_.push_back(l);
}

void GameUniverse::applyImpulse(SmallBody* a, SmallBody* b, vector2p im,
        vector2p pos) {
    a->applyImpulseAt(im, pos);
    b->applyImpulseAt(-im, pos + a->getPosition() - b->getPosition());
}

void GameUniverse::applyAngularImpulse(SmallBody* a, SmallBody* b, phys_t im) {
    a->applyAngularImpulse(im);
    b->applyAngularImpulse(-im);
}

Link::Link(SmallBody* a, SmallBody* b) :
    a_(a), b_(b) {
}

FixtureSpring::FixtureSpring(SmallBody* a, SmallBody* b, phys_t lStiff,
        phys_t lDamp, phys_t aStiff, phys_t aDamp) :
    Link(a, b), lStiff_(lStiff), lDamp_(lDamp), aStiff_(aStiff), aDamp_(aDamp),
            position_(vector2p()(0, 0)), orientation_(0.0) {
}

void FixtureSpring::setPosition(vector2p position) {
    position_ = position;
}

void FixtureSpring::setOrientation(phys_t orientation) {
    orientation_ = orientation;
}

state2p FixtureSpring::getTargetState() {
    phys_t orientation = a_->getOrientation();
    vector2p offset = position_.rotated(
            vector2p()(cos(orientation), sin(orientation)));
    state2p r;
    r.p = a_->getPosition() + offset;
    r.v = a_->getVelocityAt(offset);
    return r;
}

void FixtureSpring::update(phys_t dt, GameUniverse* u) {
    state2p target = getTargetState();
    state2p d = b_->getState() - target;
    vector2p n = d.p.unit();
    vector2p im = (d.p * lStiff_ + n * (d.v * n * lDamp_)) * dt;
    u->applyImpulse(a_, b_, im, target.p - a_->getPosition());
    phys_t da = b_->getOrientation() - a_->getOrientation() + orientation_;
    da = remainder<phys_t> (da, 2 * PI);
    phys_t dav = b_->getAngularVelocity() - a_->getAngularVelocity();
    u->applyAngularImpulse(a_, b_, (da * aStiff_ + dav * aDamp_) * dt);
}
