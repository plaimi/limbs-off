/*
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
 * Copyright (C) 2012, 2013 Alexander Berntsen <alexander@plaimi.net>
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
#include "collision_handler.hxx"
#include "geometry.hxx"
#include "physics/game_physics.hxx"

SmallBody::SmallBody(state2p s, phys_t mass, phys_t orientation, phys_t av,
        phys_t moi, Shape<phys_t>* shape, int collisionGroup) :
        Body(s, mass, orientation, av, moi, shape),
        nextState_(getBodyState()),
        collisionGroup_(collisionGroup) {
}

bool SmallBody::interact(AstroBody* b, double dt, vector2p& p, vector2p& im) {
    return false;
}

void SmallBody::applyImpulseAndRewind(vector2p impulse, vector2p pos, phys_t dt,
        phys_t fraction) {
    applyImpulseAt(impulse, pos);
    updateState(dt * (1 - fraction));
    nextState_ = getBodyState();
    updateState(-dt);
}

void SmallBody::updateState(phys_t dt) {
    s_.p += s_.v * dt;
    orientation_ += av_ * dt;
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
        Body(state2p()(0.0, 0.0, 0.0, 0.0), gm / G, 0.0, av, moi, shape,
                true),
        gm(gm) {
}

GameUniverse::GameUniverse(AstroBody* planet) :
        planet_(planet),
        smallBodies_(),
        links_() {
}

void GameUniverse::update(phys_t dt) {
    planet_->orientation_ = remainder<phys_t> (
            planet_->orientation_ + dt * planet_->av_, 2 * PI);
    const phys_t dts[] = { 0.5 * dt, 0.5 * dt, dt };
    std::vector<SmallBody*>::iterator ib, ib2;
    CollisionQueue collisions;
    for (ib = smallBodies_.begin(); ib < smallBodies_.end(); ++ib) {
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
        bodystate bs = b->getNextState(dt), ns = bs;
        b->nextState_ = ns;
        phys_t t;
        vector2p p, n;
        if (collide(planet_, b, np, bs, t, p, n)) {
            Collision c = { t, planet_, b, np, bs, p, n };
            collisions.add(c);
        }
        for (ib2 = smallBodies_.begin(); ib2 < ib; ++ib2) {
            SmallBody* b2 = *ib2;
            if (b->collisionGroup_ == b2->collisionGroup_)
                continue;
            bs = ns;
            bodystate b2s = b2->nextState_;
            if (collide(b2, b, b2s, bs, t, p, n)) {
                Collision c = { t, b2, b, b2s, bs, p, n };
                collisions.add(c);
            }
        }
    }
    while (!collisions.empty()) {
        Collision c = collisions.pop();
        CollisionHandler* collisionHandler = CollisionHandler::getInstance();
        SmallBody* body1 = (SmallBody*) c.body[1];
        body1->setBodyState(c.state[1]);
        vector2p pos1 = c.position + c.state[0].l.p - c.state[1].l.p;
        vector2p impulse;
        if (c.body[0]->getInvMass() == 0) {
            impulse = -bounce1(body1, c.body[0], pos1, c.position, -c.normal,
                    0.8, 0.2, 0.1 / (1.0 / pos1.length() + 1.0 /
                    c.position.length()), 0.05);
        }
        else {
            SmallBody* body0 = (SmallBody*) c.body[0];
            body0->setBodyState(c.state[0]);
            impulse = bounce2(body0, body1, c.position, pos1, c.normal,
                    1.25, 0.2, 0.02, 0.05);
            body0->applyImpulseAndRewind(impulse, c.position, dt, c.time);
        }
        body1->applyImpulseAndRewind(-impulse, pos1, dt, c.time);
        collisionHandler->collide(c.body[0], body1, impulse.length());
        // TODO: Update collision queue
    }
    for (ib = smallBodies_.begin(); ib < smallBodies_.end(); ++ib) {
        SmallBody* b = *ib;
        b->setBodyState(b->nextState_);
        vector2p pg, im;
        if (b->interact(planet_, dt, pg, im))
            b->applyImpulseAt(im, pg - b->getPosition());
    }
    std::vector<Link*>::iterator il;
    for (il = links_.begin(); il < links_.end(); ++il)
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
        a_(a),
        b_(b) {
}

FixtureSpring::FixtureSpring(SmallBody* a, SmallBody* b, phys_t lStiff,
        phys_t lDamp, phys_t aStiff, phys_t aDamp) :
    Link(a, b), lStiff_(lStiff), lDamp_(lDamp), aStiff_(aStiff), aDamp_(aDamp),
            position_(vector2p()(0, 0)), orientation_(0.0), enabled_(true) {
}

void FixtureSpring::setEnabled(bool status) {
    enabled_ = status;
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
    if (!enabled_)
        return;
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
