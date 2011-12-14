#ifndef CHARACTER_H_
#define CHARACTER_H_ value

#include "game_physics.h"
#include "physics.h"
#include "game_graphics_gl.h"

class Character {
public:
    class CharacterBody: public SmallBody {
    public:
        CharacterBody(Character* parent, state2p state, phys_t mass,
                phys_t orientation, phys_t angVel, phys_t inertiaMoment,
                Shape<phys_t>* shape);
    protected:
        Character* parent_;
        bool interact(AstroBody* b, double dt, vector2p& p, vector2p& im);
    };
    Character(state2p state, phys_t mass, phys_t orientation, phys_t angVel,
            phys_t inertiaMoment, Shape<phys_t>* shape);
    void addToUniverse(GameUniverse* u);
    // Return current velocity (_vel)
    double getVel();
    vector2p getPosition();
    // Procedures for setting request states
    void crouch(bool state);
    void fire(bool state);
    void leftKick(bool state);
    void leftPunch(bool state);
    void jump(bool state);
    void move(double vel);
    void rightKick(bool state);
    void rightPunch(bool state);
    // Update power meters
    void update(double deltaTime);
private:
    CharacterBody body_;
    // Requests
    bool crouch_, fire_, jump_, leftKick_, leftPunch_, rightKick_, rightPunch_;
    // Power meters
    double powerFire_, powerJump_, powerLeftKick_, powerLeftPunch_,
            powerRightKick_, powerRightPunch_;
    // Velocity and direction
    double vel_;
    friend class CharacterGraphic;
};

class CharacterGraphic: public StackGraphic {
public:
    CharacterGraphic(Character* c);
private:
    Character* c_;
    GraphicFixture bodyFixture_;
    ColorModifier bodyColor_;
    TestDisk body_;
};

#endif
