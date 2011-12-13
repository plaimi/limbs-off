#include "character.h"

Character::Character(state2p state, phys_t mass, phys_t orientation,
        phys_t angVel, phys_t inertiaMoment, Shape<phys_t>* shape) :
            // Physical object
            body_(this, state, mass, orientation, angVel, inertiaMoment, shape),
            // Request states
            crouch_(false), fire_(false), jump_(false), leftKick_(false),
            leftPunch_(false), rightKick_(false), rightPunch_(false),
            // Power meters
            powerFire_(0.0), powerJump_(0.0), powerLeftKick_(0.0),
            powerLeftPunch_(0.0), powerRightKick_(0.0), powerRightPunch_(0.0),
            // Velocity
            vel_(0) {
}

void Character::addToUniverse(GameUniverse* u) {
    u->addBody(&body_);
}

double Character::getVel() {
    return vel_;
}

vector2p Character::getPosition() {
    return body_.getPosition();
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
    vector2p legB = ~legA * (0.5 * parent_->vel_);
    phys_t radius = ((Circle<phys_t>*) (body->getShape()))->getRadius();
    phys_t radiusSqr = radius * radius;
    if (intersectLineCircle<phys_t> (legA, legB, radiusSqr, t1, t2)) {
        vector2p interactBody = legA * (1 - t1) + legB * t1;
        interactPoint = interactBody + posBody;
        vector2p interactCharacter = interactPoint - posCharacter;
        phys_t distanceSqr = interactCharacter.squared();
        phys_t leg = 2.0 * (1.0 - parent_->powerJump_);
        if (distanceSqr < leg * leg) {
            phys_t distance = sqrt(distanceSqr);
            vector2p n = interactCharacter / distance;
            impulse = -n * (2000.0 * (leg - distance) + max(0.0,
                    getMomentum() * n * 8.0)) * deltaTime;
            return true;
        }
    }
    return false;
}

CharacterGraphic::CharacterGraphic(Character* c) :
    c_(c), bodyDisk_(1.0, 16), body_(&c->body_, &bodyDisk_, 0, 0, 0) {
}

void CharacterGraphic::draw() {
    glColor3f(0.2, 0.2, 0.8);
    body_.draw();
}
