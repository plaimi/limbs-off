#ifndef CHARACTER_H_
#define CHARACTER_H_ value

#include "game_physics.h"
#include "physics.h"

class Character: public SmallBody {
public:
    Character(phys_t mass, state2p state, phys_t moi, phys_t o, phys_t av,
            Shape<phys_t>* shape);
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
