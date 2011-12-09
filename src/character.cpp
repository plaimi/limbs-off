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
    powerJump_ = state;
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

bool Character::interact(AstroBody* b, double dt, vector2p& p, vector2p& im) {
    phys_t t1, t2;
    vector2p pc = getPosition(), pb = b->getPosition();
    vector2p la = pc - pb, lb = ~la * (0.5 * vel_);
    phys_t r = ((Circle<phys_t>*) (b->getShape()))->getRadius();
    phys_t rr = r * r;
    if (intersectLineCircle<phys_t> (la, lb, rr, t1, t2)) {
        vector2p ib = la * (1 - t1) + lb * t1;
        p = ib + pb;
        vector2p ic = p - pc;
        phys_t dd = ic.squared();
        phys_t l = 2.0 * (1.0 - powerJump_);
        if (dd < l * l) {
            phys_t d = sqrt(dd);
            vector2p n = ic / d;
            im = -n * (2000 * (l - d) + max(0.0, getMomentum() * n * 8)) * dt;
            return true;
        }
    }
    return false;
}
