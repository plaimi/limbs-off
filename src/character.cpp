#include "character.h"

Character::Character(state2p state, phys_t mass, phys_t orientation,
        phys_t angVel, phys_t inertiaMoment, Shape<phys_t>* shape) :
            // Physical object
            SmallBody(state, mass, orientation, angVel, inertiaMoment, shape),
            // Request states
            crouch_(false), fire_(false), jump_(false), leftKick_(false),
            leftPunch_(false), rightKick_(false), rightPunch_(false),
            // Power meters
            powerFire_(0.0), powerJump_(0.0), powerLeftKick_(0.0),
            powerLeftPunch_(0.0), powerRightKick_(0.0), powerRightPunch_(0.0),
            // Velocity
            vel_(0) {
}

double Character::getVel() {
    return vel_;
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

bool Character::interact(AstroBody* body, double deltaTime, vector2p& interactPoint, vector2p& impulse) {
    phys_t t1, t2;
    vector2p posCharacter = getPosition(), posBody = body->getPosition();
    vector2p legA = posCharacter - posBody;
    vector2p legB = ~legA * (0.5 * vel_);
    phys_t radius = ((Circle<phys_t>*) (body->getShape()))->getRadius();
    phys_t radiusSqr = radius * radius;
    if (intersectLineCircle<phys_t> (legA, legB, radiusSqr, t1, t2)) {
        vector2p interactBody = legA * (1 - t1) + legB * t1;
        interactPoint = interactBody + posBody;
        vector2p interactCharacter = interactPoint - posCharacter;
        phys_t distanceSqr = interactCharacter.squared();
        phys_t leg = 2.0 * (1.0 - powerJump_);
        if (distanceSqr < leg * leg) {
            phys_t distance = sqrt(distanceSqr);
            vector2p n = interactCharacter / distance;
            impulse = -n * (2000.0 * (leg - distance) + max(0.0, getMomentum() * n * 8.0)) * deltaTime;
            return true;
        }
    }
    return false;
}
