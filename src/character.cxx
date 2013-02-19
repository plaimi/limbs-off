/*
 * Copyright (C) 2011, 2012, 2013 Alexander Berntsen <alexander@plaimi.net>
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
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

#include "character.hxx"
#include "collision_handler.hxx"
#include "get_texture.hxx"

int Character::_collisionGroup_ = 0;

Character::Character(state2p state, phys_t orientation, Material* materialBody,
        Material* materialHead, Material* materialLimbs,
        Material* materialLimbsOff) :
        shapeBody_(0.25),
        shapeHead_(0.15),
        shapeFoot_(0.075),
        shapeHand_(0.1),
        // Physical object
        body_(this, state, 100, orientation, 0, momentInertia(100, 0.25, 0.4),
                &shapeBody_, materialBody, _collisionGroup_),
        head_(getStateAt(vector2p()(0.0, 0.40)), 3, orientation, 0,
                momentInertia(2.5, 0.15, 0.4), &shapeHead_, materialHead,
                _collisionGroup_),
        footBack_(getStateAt(vector2p()(0.0, -0.40)), 1, orientation, 0,
                momentInertia(1, 0.075, 0.4), &shapeFoot_, materialLimbs,
                _collisionGroup_),
        footFront_(getStateAt(vector2p()(0.0, -0.40)), 1, orientation, 0,
                momentInertia(1, 0.075, 0.4), &shapeFoot_, materialLimbs,
                _collisionGroup_),
        handBack_(state, 2, orientation, 0, momentInertia(2, 0.1, 0.4),
                &shapeHand_, materialLimbs, _collisionGroup_),
        handFront_(state, 2, orientation, 0, momentInertia(2, 0.1, 0.4),
                &shapeHand_, materialLimbs, _collisionGroup_),
        // Links
        neck_(&body_, &head_, 1500.0, 150.0, 1.0, 1.0),
        legBack_(&body_, &footBack_, 200.0, 20.0, 1.0, 1.0),
        legFront_(&body_, &footFront_, 200.0, 20.0, 1.0, 1.0),
        armBack_(&body_, &handBack_, 200.0, 20.0, 1.0, 1.0),
        armFront_(&body_, &handFront_, 200.0, 20.0, 1.0, 1.0),
        // Velocity
        vel_(0),
        // State
        dead_(false),
        materialLimbsOff_(materialLimbsOff) {
    actions_[LEFT].intention = true;
    neck_.setPosition(vector2p()(0.0, 0.40));
    legBack_.setPosition(vector2p()(0.0, -0.40));
    legFront_.setPosition(vector2p()(0.0, -0.40));
    ++_collisionGroup_;
    CollisionHandler* collisionHandler = CollisionHandler::getInstance();
    collisionHandler->monitor(&body_, this);
    collisionHandler->monitor(&head_, this);
}

bool Character::isDead() {
    return dead_;
}

char Character::getOrientation() {
    return actions_[LEFT].intention ? 'l' : 'r';
}

double Character::getVel() {
    return vel_;
}

phys_t Character::getMass() {
    return body_.getMass();
}


state2p Character::getState() {
    return body_.getState();
}

void Character::addToUniverse(GameUniverse* u) {
    u->addBody(&body_);
    u->addBody(&head_);
    u->addBody(&footBack_);
    u->addBody(&footFront_);
    u->addBody(&handBack_);
    u->addBody(&handFront_);
    u->addLink(&neck_);
    u->addLink(&legBack_);
    u->addLink(&legFront_);
    u->addLink(&armBack_);
    u->addLink(&armFront_);
}

void Character::die() {
    if (dead_)
        return;
    dead_ = true;
    // Disable all the links, making the game live up to its name.
    neck_.setEnabled(false);
    legFront_.setEnabled(false);
    legBack_.setEnabled(false);
    armBack_.setEnabled(false);
    armFront_.setEnabled(false);
    footBack_.setMaterial(materialLimbsOff_);
    footFront_.setMaterial(materialLimbsOff_);
    handBack_.setMaterial(materialLimbsOff_);
    handFront_.setMaterial(materialLimbsOff_);
}

void Character::crouch(bool state) {
    actions_[CROUCH].intention = state;
}

void Character::fire(bool state) {
    actions_[FIRE].intention = state;
}

void Character::hit(Body* part, phys_t dmg) {
    body_.changeMass(dmg);
}

void Character::leftKick(bool state) {
    actions_[LKICK].intention = state;
}

void Character::leftPunch(bool state) {
    actions_[LPUNCH].intention = state;
}

void Character::jump(bool state) {
    actions_[JUMP].intention = state;
}

void Character::moveLeft(double vel) {
    actions_[LEFT].power = vel;
    vel_ = !actions_[LEFT].power ? actions_[RIGHT].power :
        -actions_[LEFT].power;
    actions_[LEFT].intention = true;
    actions_[RIGHT].intention = false;
}

void Character::moveRight(double vel) {
    actions_[RIGHT].power = vel;
    vel_ = !actions_[RIGHT].power ? -actions_[LEFT].power :
        actions_[RIGHT].power;
    actions_[LEFT].intention = false;
    actions_[RIGHT].intention = true;
}

void Character::rightKick(bool state) {
    actions_[RKICK].intention = state;
}

void Character::rightPunch(bool state) {
    actions_[RPUNCH].intention = state;
}

void Character::update(double deltaTime) {
    double decay = pow(.2, deltaTime);
    // If crouching, raise the crouching power to a max of 0.75.
    // If not crouching, easily uncrouch without jumping.
    actions_[CROUCH].power = actions_[CROUCH].intention ?
            min(20.0 / body_.getMass(), 1.5 * (1.0 - decay) +
            actions_[CROUCH].power * decay) : max(0.0, actions_[CROUCH].power
                - 3.0 * deltaTime );
    // Cap jump power to 1.0 by forcejumping.
    if (actions_[JUMP].power >= 1.0)
        actions_[JUMP].intention = false;
    // If jumping, raise jump power. If not, set jump power to 0.
    actions_[JUMP].power = actions_[JUMP].intention ? 1.5 * (1.0 - decay) +
        actions_[JUMP].power * decay : 0.0;
    for (Action* i = &actions_[LPUNCH];; i = &actions_[RPUNCH]) {
        i->power = i->intention ? 250.0 * (1.0 - decay) + i->power * decay :
            0.0;
        if (i == &actions_[RPUNCH])
            break;
    }
}

state2p Character::getStateAt(vector2p p) {
    phys_t angle = body_.getOrientation();
    vector2p offset = p.rotated(vector2p()(cos(angle), sin(angle)));
    return state2p()(body_.getPosition() + offset, body_.getVelocityAt(offset));
}

Character::CharacterBody::CharacterBody(Character* parent, state2p state,
        phys_t mass, phys_t orientation, phys_t angVel, phys_t inertiaMoment,
        Shape<phys_t>* shape, Material* material, int collisionGroup) :
    SmallBody(state, mass, orientation, angVel, inertiaMoment, shape,
            material, collisionGroup), parent_(parent), walkCycle_(0.0) {
}

bool Character::CharacterBody::interact(AstroBody* body, double deltaTime,
        vector2p& interactPoint, vector2p& impulse) {
    if (parent_->isDead())
        return false;
    // FIXME: Way too much happening here. Move some of the code elsewhere.
    phys_t t1, t2;
    vector2p posCharacter = getPosition(), posBody = body->getPosition();
    vector2p legA = posCharacter - posBody;
    phys_t hVel = (getVelocity() - body->getVelocityAt(legA)) / legA.unit();
    phys_t angle = legA.angle() - PI / 2 - getOrientation();
    phys_t accel = clampmag(hVel / 8.0 - parent_->vel_, 1.0);
    angle = clampmag(remainder(angle, PI * 2) + accel * PI / 4, PI / 2);
    walkCycle_ = remainder(walkCycle_ - deltaTime * hVel * 5.0, PI * 2);
    // Calculate the leg length. Depends on jump/crouch power if any.
    phys_t leg = 0.40 - 0.15 * max(parent_->actions_[CROUCH].power,
            parent_->actions_[JUMP].power);
    vector2p feetOrigin = vector2p::fromAngle(angle - PI / 2) * (leg - 0.05);
    vector2p feetOffset = vector2p::fromAngle(walkCycle_) * 0.15;
    parent_->legBack_.setPosition(feetOrigin + feetOffset);
    parent_->legFront_.setPosition(feetOrigin - feetOffset);
    {
        vector2p p;
        int f = (parent_->getOrientation() == 'r') ? 1 : -1;
        Action* i;
        FixtureSpring* j;
        for (i = &(parent_->actions_[LPUNCH]),
                j = f >> 1 ? &(parent_->armFront_) :
                &(parent_->armBack_);; i = &(parent_->actions_[RPUNCH]),
                j = j == &(parent_->armFront_) ? &(parent_->armBack_) :
                &(parent_->armFront_)) {
            j->setPosition(p(i->intention ? 0.25 * -f : i->power * f, 0));
            if (i == &(parent_->actions_[RPUNCH]))
                break;
        }
    }
    angle += PI / 2 + getOrientation();
    vector2p legB = legA - vector2p()(cos(angle), sin(angle));
    phys_t bodyRadius = ((Circle<phys_t>*) (body->getShape()))->getRadius();
    phys_t bodyRadiusSqr = bodyRadius * bodyRadius;
    if (intersectLineCircle<phys_t> (legA, legB, bodyRadiusSqr, t1, t2) &&
            t1 > 0 && t1 < leg) {
        vector2p interactBody = legA * (1 - t1) + legB * t1;
        interactPoint = interactBody + posBody;
        vector2p interactCharacter = interactPoint - posCharacter;
        vector2p n = interactCharacter / t1;
        impulse = - n * pow(.75, abs(hVel) - 8.) * (8000. * (leg - t1) + max(.0,
                    getMomentum() * n * 50.)) * deltaTime;
        phys_t da = remainder(getOrientation() - atan2(n.x, -n.y), 2 * PI);
        phys_t dav = getAngularVelocity() + getVelocity() / legA
                / legA.squared();
        vector2p na = legA.unit();
        phys_t in = na * impulse;
        phys_t balancing = (da * 500 + dav * 200) * deltaTime;
        phys_t ip = clampmag(~na * impulse + balancing, 1.8 * in);
        impulse = na * in + ~na * ip;
        return true;
    }
    phys_t altitude = legA.length() - bodyRadius;
    // Prevent character from floating around in space... by killing it.
    if (altitude > 2.0)
        changeMass(deltaTime * altitude);
    return false;
}

void Character::CharacterBody::changeMass(phys_t delta) {
    // Make sure the mass doesn't drop too low, messing up our fragile system.
    phys_t deathCap = 3.0;
    mass_ = max(mass_ - delta, deathCap);
    invMass_ = 1.0 / mass_;
    phys_t radius = 0.25 * pow(mass_ / 100.0, 1.0 / deathCap);
    moi_ = momentInertia(mass_, radius, 0.4);
    // Update the graphics to reflect the logic
    ((Circle<phys_t>*) shape_)->setRadius(radius);
    if (getMass() <= deathCap)
        parent_->die();
}

Character::Action::Action() :
        intention(false),
        power(0.0) {
}

CharacterGraphic::CharacterGraphic(Character* c) :
        c_(c),
        body_(&bodyLeft_), head_(&headLeft_),
        bodyLeft_(getTexture(PACKAGE_GFX_DIR "character_body_left.png"), 1.0,
                1.0),
        bodyRight_(getTexture(PACKAGE_GFX_DIR "character_body_right.png"),
                1.0, 1.0),
        headLeft_(getTexture(PACKAGE_GFX_DIR "character_head_left.png"), 0.15,
                0.15),
        headRight_(getTexture(PACKAGE_GFX_DIR "character_head_right.png"),
                0.15, 0.15),
        footFront_(getTexture(PACKAGE_GFX_DIR "character_foot.png"), 0.075,
                0.075),
        footBack_(getTexture(PACKAGE_GFX_DIR "character_foot.png"), 0.075,
                0.075),
        handFront_(getTexture(PACKAGE_GFX_DIR "character_hand.png"), 0.1,
                0.1),
        handBack_(getTexture(PACKAGE_GFX_DIR "character_hand.png"), 0.1, 0.1),
        bodyFixture_(&c->body_),
        headFixture_(&c->head_),
        footBackFixture_(&c->footBack_),
        footFrontFixture_(&c->footFront_),
        handBackFixture_(&c->handBack_),
        handFrontFixture_(&c->handFront_),
        bodyColor_(colour_),
        scaler_(c->body_.getShape()),
        orientation_(c->getOrientation()) {
    static int n = 0;
    int m = n % 3, d = n / 3, mm = d / 2 % 4, dd = d / 8;
    for (int i = 0; i < 3; i++)
        colour_[i] = (3 * ((m == i) ^ d & 1) ^ ((mm == i + 1) ^ dd & 1)) / 4.0;
    ++n;

    bodyLeft_.addModifier(&scaler_);
    bodyLeft_.addModifier(&bodyFixture_);
    bodyLeft_.addModifier(&bodyColor_);
    bodyRight_.addModifier(&scaler_);
    bodyRight_.addModifier(&bodyFixture_);
    bodyRight_.addModifier(&bodyColor_);
    headLeft_.addModifier(&headFixture_);
    headLeft_.addModifier(&bodyColor_);
    headRight_.addModifier(&headFixture_);
    headRight_.addModifier(&bodyColor_);
    footBack_.addModifier(&footBackFixture_);
    footBack_.addModifier(&bodyColor_);
    footFront_.addModifier(&footFrontFixture_);
    footFront_.addModifier(&bodyColor_);
    handBack_.addModifier(&handBackFixture_);
    handBack_.addModifier(&bodyColor_);
    handFront_.addModifier(&handFrontFixture_);
    handFront_.addModifier(&bodyColor_);
    addGraphic(&handBack_);
    addGraphic(&footBack_);
    addGraphic(body_);
    addGraphic(head_);
    addGraphic(&footFront_);
    addGraphic(&handFront_);
}

bool CharacterGraphic::updateOrientation() {
    char orientation = c_->getOrientation();
    if (orientation_ == orientation)
        return false;
    orientation_ = orientation;
    return true;
}

ColorModifier* CharacterGraphic::getColourModifier() {
    return &bodyColor_;
}

void CharacterGraphic::update() {
    if (updateOrientation()) {
        // Remove old graphics.
        std::size_t oldBody = removeGraphic(body_);
        std::size_t oldHead = removeGraphic(head_);
        // Determine whether going left or right.
        if (orientation_ == 'l') {
            head_ = &headLeft_;
            body_ = &bodyLeft_;
        }
        else {
            head_ = &headRight_;
            body_ = &bodyRight_;
        }
        // Add new graphics.
        addGraphic(head_, oldHead);
        addGraphic(body_, oldBody);
    }
}
