#include "character.h"

Character::Character(state2p state, phys_t mass, phys_t orientation, phys_t angVel,
        phys_t inertiaMoment, Shape<phys_t>* shape) :
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
};

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
