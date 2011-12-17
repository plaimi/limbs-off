#include "character.h"

namespace {
const float COL_BODY[] = { 0.2, 0.2, 0.8 };
}

Character::Character(state2p state, phys_t orientation) :
            shapeBody_(0.2),
            shapeHead_(0.15),
            // Physical object
            body_(this, state, 25, orientation, 0, momentInertia(25, 0.2, 0.4),
                    &shapeBody_),
            head_(getStateAt(vector2p()(0.0, 0.40)), 3, orientation, 0,
                    momentInertia(2, 0.15, 0.4), &shapeHead_),
            neck_(&body_, &head_, 1000.0, 1000.0, 1.0, 1.0),
            // Request states
            crouch_(false), fire_(false), jump_(false), leftKick_(false),
            leftPunch_(false), rightKick_(false), rightPunch_(false),
            // Power meters
            powerFire_(0.0), powerJump_(0.0), powerLeftKick_(0.0),
            powerLeftPunch_(0.0), powerRightKick_(0.0), powerRightPunch_(0.0),
            // Velocity
            vel_(0) {
    neck_.setPosition(vector2p()(0.0, 0.40));
}

void Character::addToUniverse(GameUniverse* u) {
    u->addBody(&body_);
    u->addBody(&head_);
    u->addLink(&neck_);
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
            parent_(parent) {
}

bool Character::CharacterBody::interact(AstroBody* body, double deltaTime,
        vector2p& interactPoint, vector2p& impulse) {
    phys_t t1, t2;
    vector2p posCharacter = getPosition(), posBody = body->getPosition();
    vector2p legA = posCharacter - posBody;
    phys_t angle = atan2(legA.y, legA.x) - PI / 2 - getOrientation();
    angle = clampmag(remainder(angle, PI * 2) - parent_->vel_ * PI / 7, PI / 2);
    angle += PI / 2 + getOrientation();
    vector2p legB = legA - vector2p()(cos(angle), sin(angle));
    phys_t radius = ((Circle<phys_t>*) (body->getShape()))->getRadius();
    phys_t radiusSqr = radius * radius;
    if (intersectLineCircle<phys_t> (legA, legB, radiusSqr, t1, t2)) {
        vector2p interactBody = legA * (1 - t1) + legB * t1;
        interactPoint = interactBody + posBody;
        vector2p interactCharacter = interactPoint - posCharacter;
        phys_t leg = 0.25 + 0.15 * (1.0 - parent_->powerJump_);
        if (t1 < leg) {
            vector2p n = interactCharacter / t1;
            impulse = -n * (8000.0 * (leg - t1) + max(0.0,
                    getMomentum() * n * 50.0)) * deltaTime;
            phys_t da = remainder(getOrientation() - atan2(-legA.x, legA.y),
                    2 * PI);
            phys_t dav = getAngularVelocity();
            impulse -= ~n * (da * 500 + dav * 200) * deltaTime;
            vector2p na = legA.unit();
            phys_t in = na * impulse;
            phys_t ip = clampmag(~na * impulse, 0.5 * in);
            impulse = na * in + ~na * ip;
            return true;
        }
    }
    return false;
}

CharacterGraphic::CharacterGraphic(Character* c) :
    c_(c), bodyFixture_(&c->body_), headFixture_(&c->head_),
            bodyColor_(COL_BODY), body_(0.2, 16), head_(0.15, 16) {
    body_.addModifier(&bodyFixture_);
    body_.getDisk()->addModifier(&bodyColor_);
    head_.addModifier(&headFixture_);
    head_.getDisk()->addModifier(&bodyColor_);
    addGraphic(&body_);
    addGraphic(&head_);
}
