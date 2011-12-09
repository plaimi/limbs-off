#ifndef CHARACTER_H_
#define CHARACTER_H_ value

#include "game_physics.h"
#include "physics.h"

class Character: public SmallBody {
public:
    Character(state2p state, phys_t mass, phys_t orientation, phys_t angVel,
            phys_t inertiaMoment, Shape<phys_t>* shape);
    // Return current velocity (_vel)
    double getVel();
    // Procedures for setting request states
    void crouch(bool state);
    void fire(bool state);
    void leftKick(bool state);
    void leftPunch(bool state);
    void jump(bool state);
    void move(double vel);
    void rightKick(bool state);
    void rightPunch(bool state);
protected:
    bool interact(AstroBody* b, double dt, vector2p& p, vector2p& im);
private:
    // Requests
    bool crouch_, fire_, jump_, leftKick_, leftPunch_, rightKick_, rightPunch_;
    // Power meters
    double powerFire_, powerJump_, powerLeftKick_, powerLeftPunch_,
        powerRightKick_, powerRightPunch_;
    // Velocity and direction
    double vel_;
};

#endif
