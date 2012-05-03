/*
 * Copyright (C) 2011, 2012 Alexander Berntsen <alexander@plaimi.net>
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

int Character::collisionGroup_ = 0;

Character::Character(state2p state, phys_t orientation) :
        shapeBody_(0.25),
        shapeHead_(0.15),
        shapeFoot_(0.075),
        shapeHand_(0.1),
        // Physical object
        body_(this, state, 100, orientation, 0, momentInertia(100, 0.25, 0.4),
                &shapeBody_, collisionGroup_),
        head_(getStateAt(vector2p()(0.0, 0.40)), 3, orientation, 0,
                momentInertia(2.5, 0.15, 0.4), &shapeHead_, collisionGroup_),
        footBack_(getStateAt(vector2p()(0.0, -0.40)), 1, orientation, 0,
                momentInertia(1, 0.075, 0.4), &shapeFoot_, collisionGroup_),
        footFront_(getStateAt(vector2p()(0.0, -0.40)), 1, orientation, 0,
                momentInertia(1, 0.075, 0.4), &shapeFoot_, collisionGroup_),
        handBack_(state, 2, orientation, 0, momentInertia(2, 0.1, 0.4),
                &shapeHand_, collisionGroup_),
        handFront_(state, 2, orientation, 0, momentInertia(2, 0.1, 0.4),
                &shapeHand_, collisionGroup_),
        // Links
        neck_(&body_, &head_, 1500.0, 150.0, 1.0, 1.0),
        legBack_(&body_, &footBack_, 200.0, 20.0, 1.0, 1.0),
        legFront_(&body_, &footFront_, 200.0, 20.0, 1.0, 1.0),
        armBack_(&body_, &handBack_, 200.0, 20.0, 1.0, 1.0),
        armFront_(&body_, &handFront_, 200.0, 20.0, 1.0, 1.0),
        // Request states
        crouch_(false), fire_(false), jump_(false), leftKick_(false),
        leftPunch_(false), rightKick_(false), rightPunch_(false),
        // Power meters
        powerCrouch_(0.0), powerFire_(0.0), powerJump_(0.0),
        powerLeftKick_(0.0), powerLeftPunch_(0.0), powerRightKick_(0.0),
        powerRightPunch_(0.0),
        // Velocity
        vel_(0), velLeft_(0), velRight_(0),
        // State
        dead_(false) {
    neck_.setPosition(vector2p()(0.0, 0.40));
    legBack_.setPosition(vector2p()(0.0, -0.40));
    legFront_.setPosition(vector2p()(0.0, -0.40));
    ++collisionGroup_;
    CollisionHandler* collisionHandler = CollisionHandler::getInstance();
    collisionHandler->monitor(&body_, this);
    collisionHandler->monitor(&head_, this);
    collisionHandler->monitor(&footBack_, this);
    collisionHandler->monitor(&footFront_, this);
    collisionHandler->monitor(&handBack_, this);
    collisionHandler->monitor(&handFront_, this);
}

bool Character::isDead() {
    return dead_;
}

double Character::getVel() {
    return vel_;
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
}

void Character::crouch(bool state) {
    crouch_ = state;
}

void Character::fire(bool state) {
    fire_ = state;
}

void Character::hit(Body* part, phys_t impulse) {
    // Ten times damage for headshots
    if (part == &head_)
        impulse *= 10.0;
    // Apply impulse as a force on the main body
    body_.changeMass(impulse / 300.0);
}

void Character::leftKick(bool state) {
    leftKick_ = state;
}

void Character::leftPunch(bool state) {
    leftPunch_ = state;
}

void Character::jump(bool state) {
    jump_ = state;
}

void Character::moveLeft(double vel) {
    velLeft_ = vel;
    vel_ = !velLeft_ ? velRight_ : -velLeft_;
}

void Character::moveRight(double vel) {
    velRight_ = vel;
    vel_ = !velRight_ ? -velLeft_ : velRight_;
}

void Character::rightKick(bool state) {
    rightKick_ = state;
}

void Character::rightPunch(bool state) {
    rightPunch_ = state;
}

void Character::update(double deltaTime) {
    double decay = pow(.2, deltaTime);
    // If crouching, raise the crouching power to a max of 0.75.
    // If not crouching, easily uncrouch without jumping.
    powerCrouch_ = crouch_ ?
            min(20.0 / body_.getMass(), 1.5 * (1.0 - decay) + powerCrouch_ * decay) :
            max(0.0, powerCrouch_ - 3.0 * deltaTime );
    // Cap jump power to 1.0 by forcejumping.
    if (powerJump_ >= 1.0)
        jump_ = false;
    // If jumping, raise jump power. If not, set jump power to 0.
    powerJump_ = jump_ ? 1.5 * (1.0 - decay) + powerJump_ * decay : 0.0;
}

state2p Character::getStateAt(vector2p p) {
    phys_t angle = body_.getOrientation();
    vector2p offset = p.rotated(vector2p()(cos(angle), sin(angle)));
    return state2p()(body_.getPosition() + offset, body_.getVelocityAt(offset));
}

Character::CharacterBody::CharacterBody(Character* parent, state2p state,
        phys_t mass, phys_t orientation, phys_t angVel, phys_t inertiaMoment,
        Shape<phys_t>* shape, int collisionGroup) :
    SmallBody(state, mass, orientation, angVel, inertiaMoment, shape,
            collisionGroup), parent_(parent), walkCycle_(0.0) {
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
    phys_t leg = 0.40 - 0.15 * max(parent_->powerCrouch_, parent_->powerJump_);
    vector2p feetOrigin = vector2p::fromAngle(angle - PI / 2) * (leg - 0.05);
    vector2p feetOffset = vector2p::fromAngle(walkCycle_) * 0.15;
    parent_->legBack_.setPosition(feetOrigin + feetOffset);
    parent_->legFront_.setPosition(feetOrigin - feetOffset);
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

CharacterGraphic::CharacterGraphic(Character* c) :
    c_(c), bodyFixture_(&c->body_), headFixture_(&c->head_),
            footBackFixture_(&c->footBack_), footFrontFixture_(&c->footFront_),
            handBackFixture_(&c->handBack_), handFrontFixture_(&c->handFront_),
            bodyColor_(colour_), scaler_(c->body_.getShape()), body_(1.0, 16),
            head_(0.15, 16), footBack_(0.075, 8), footFront_(0.075, 8),
            handBack_(0.1, 8), handFront_(0.1, 8) {
    // Temporary differentiation of characters.
    // TODO: Use sprites.
    static int n = 0;
    int m = n % 3, d = n / 3, mm = d / 2 % 4, dd = d / 8;
    for (int i = 0; i < 3; i++)
        colour_[i] = (3 * ((m == i) ^ d & 1) ^ ((mm == i + 1) ^ dd & 1)) / 4.0;
    ++n;

    body_.addModifier(&scaler_);
    body_.addModifier(&bodyFixture_);
    body_.getDisk()->addModifier(&bodyColor_);
    head_.addModifier(&headFixture_);
    head_.getDisk()->addModifier(&bodyColor_);
    footBack_.addModifier(&footBackFixture_);
    footBack_.getDisk()->addModifier(&bodyColor_);
    footFront_.addModifier(&footFrontFixture_);
    footFront_.getDisk()->addModifier(&bodyColor_);
    handBack_.addModifier(&handBackFixture_);
    handBack_.getDisk()->addModifier(&bodyColor_);
    handFront_.addModifier(&handFrontFixture_);
    handFront_.getDisk()->addModifier(&bodyColor_);
    addGraphic(&handBack_);
    addGraphic(&footBack_);
    addGraphic(&body_);
    addGraphic(&head_);
    addGraphic(&footFront_);
    addGraphic(&handFront_);
}
