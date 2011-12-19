#include "character.h"

namespace {
const float COL_BODY[] = { 0.2, 0.2, 0.8 };
}

Character::Character(state2p state, phys_t orientation) :
            shapeBody_(0.2),
            shapeHead_(0.15),
            shapeFoot_(0.05),
            shapeHand_(0.075),
            // Physical object
            body_(this, state, 25, orientation, 0, momentInertia(25, 0.2, 0.4),
                    &shapeBody_),
            head_(getStateAt(vector2p()(0.0, 0.40)), 3, orientation, 0,
                    momentInertia(3, 0.15, 0.4), &shapeHead_),
            footBack_(getStateAt(vector2p()(0.0, -0.40)), 1, orientation, 0,
                    momentInertia(1, 0.05, 0.4), &shapeFoot_),
            footFront_(getStateAt(vector2p()(0.0, -0.40)), 1, orientation, 0,
                    momentInertia(1, 0.05, 0.4), &shapeFoot_),
            handBack_(state, 2, orientation, 0, momentInertia(2, 0.075, 0.4),
                    &shapeHand_),
            handFront_(state, 2, orientation, 0, momentInertia(2, 0.075, 0.4),
                    &shapeHand_),
            neck_(&body_, &head_, 1000.0, 1000.0, 1.0, 1.0),
            legBack_(&body_, &footBack_, 200.0, 20.0, 1.0, 1.0),
            legFront_(&body_, &footFront_, 200.0, 20.0, 1.0, 1.0),
            armBack_(&body_, &handBack_, 200.0, 20.0, 1.0, 1.0),
            armFront_(&body_, &handFront_, 200.0, 20.0, 1.0, 1.0),
            // Request states
            crouch_(false), fire_(false), jump_(false), leftKick_(false),
            leftPunch_(false), rightKick_(false), rightPunch_(false),
            // Power meters
            powerFire_(0.0), powerJump_(0.0), powerLeftKick_(0.0),
            powerLeftPunch_(0.0), powerRightKick_(0.0), powerRightPunch_(0.0),
            // Velocity
            vel_(0) {
    neck_.setPosition(vector2p()(0.0, 0.40));
    legBack_.setPosition(vector2p()(0.0, -0.40));
    legFront_.setPosition(vector2p()(0.0, -0.40));
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

double Character::getVel() {
    return vel_;
}

state2p Character::getState() {
    return body_.getState();
}

void Character::crouch(bool state) {
    crouch_ = state;
}

void Character::fire(bool state) {
    fire_ = state;
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

void Character::move(double vel) {
    vel_ = vel;
}

void Character::rightKick(bool state) {
    rightKick_ = state;
}

void Character::rightPunch(bool state) {
    rightPunch_ = state;
}

void Character::update(double deltaTime) {
    double decay = pow(.2, deltaTime);
    if (jump_) {
        if (powerJump_ >= 1.0) {
            jump_ = false;
        }
        powerJump_ = 1.5 * (1.0 - decay) + powerJump_ * decay;
    }
    if (!jump_) {
        powerJump_ = 0.0;
    }
}

state2p Character::getStateAt(vector2p p) {
    phys_t angle = body_.getOrientation();
    vector2p offset = p.rotated(vector2p()(cos(angle), sin(angle)));
    return state2p()(body_.getPosition() + offset, body_.getVelocityAt(offset));
}

Character::CharacterBody::CharacterBody(Character* parent, state2p state,
        phys_t mass, phys_t orientation, phys_t angVel, phys_t inertiaMoment,
        Shape<phys_t>* shape) :
    SmallBody(state, mass, orientation, angVel, inertiaMoment, shape),
            parent_(parent), walkCycle_(0.0) {
}

bool Character::CharacterBody::interact(AstroBody* body, double deltaTime,
        vector2p& interactPoint, vector2p& impulse) {
    // FIXME: Way too much happening here. Move some of the code elsewhere.
    phys_t t1, t2;
    vector2p posCharacter = getPosition(), posBody = body->getPosition();
    vector2p legA = posCharacter - posBody;
    phys_t hVel = (getVelocity() - body->getVelocityAt(legA)) / legA.unit();
    phys_t angle = legA.angle() - PI / 2 - getOrientation();
    phys_t accel = clampmag(hVel / 8.0 - parent_->vel_, 1.0);
    angle = clampmag(remainder(angle, PI * 2) + accel * PI / 8, PI / 2);
    walkCycle_ = remainder(walkCycle_ - deltaTime * hVel * 5.0, PI * 2);
    phys_t leg = 0.25 + 0.15 * (1.0 - parent_->powerJump_);
    vector2p feetOrigin = -vector2p::fromAngle(-angle) * (leg - 0.05);
    vector2p feetOffset = vector2p::fromAngle(-walkCycle_) * 0.15;
    parent_->legBack_.setPosition(feetOrigin + feetOffset);
    parent_->legFront_.setPosition(feetOrigin - feetOffset);
    angle += PI / 2 + getOrientation();
    vector2p legB = legA - vector2p()(cos(angle), sin(angle));
    phys_t radius = ((Circle<phys_t>*) (body->getShape()))->getRadius();
    phys_t radiusSqr = radius * radius;
    if (intersectLineCircle<phys_t> (legA, legB, radiusSqr, t1, t2) && t1 > 0
            && t1 < leg) {
        vector2p interactBody = legA * (1 - t1) + legB * t1;
        interactPoint = interactBody + posBody;
        vector2p interactCharacter = interactPoint - posCharacter;
        vector2p n = interactCharacter / t1;
        impulse = -n * (8000.0 * (leg - t1)
                + max(0.0, getMomentum() * n * 50.0)) * deltaTime;
        phys_t da = remainder(getOrientation() - atan2(n.x, -n.y), 2 * PI);
        phys_t dav = getAngularVelocity() + getVelocity() / legA
                / legA.squared();
        vector2p na = legA.unit();
        phys_t in = na * impulse;
        phys_t balancing = (da * 500 + dav * 200) * deltaTime;
        phys_t ip = clampmag(~na * impulse + balancing, 0.8 * in);
        impulse = na * in + ~na * ip;
        return true;
    }
    return false;
}

CharacterGraphic::CharacterGraphic(Character* c) :
    c_(c), bodyFixture_(&c->body_), headFixture_(&c->head_),
            footBackFixture_(&c->footBack_), footFrontFixture_(&c->footFront_),
            handBackFixture_(&c->handBack_), handFrontFixture_(&c->handFront_),
            bodyColor_(COL_BODY), body_(0.2, 16), head_(0.15, 16),
            footBack_(0.05, 8), footFront_(0.05, 8), handBack_(0.075, 8),
            handFront_(0.075, 8) {
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
