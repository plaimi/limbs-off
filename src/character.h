/*
 * Copyright (C) 2011, 2012 Alexander Berntsen <alexander@plaimi.net>
 * Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
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

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "graphics/game_graphics_gl.h"
#include "physics/game_physics.h"

class Character {
public:
    class CharacterBody: public SmallBody {
    public:
        CharacterBody(Character* parent, state2p state, phys_t mass,
                phys_t orientation, phys_t angVel, phys_t inertiaMoment,
                Shape<phys_t>* shape, int collisionGroup);
        void changeMass(phys_t delta);
    protected:
        Character* parent_;
        phys_t walkCycle_;
        bool interact(AstroBody* b, double dt, vector2p& p, vector2p& im);
    };
    Character(state2p state, phys_t orientation);
    void addToUniverse(GameUniverse* u);
    bool isDead();
    double getVel();
    state2p getState();
    void crouch(bool state);
    /** Dismantle character upon death. */
    void die();
    void fire(bool state);
    /** Process collisions. */
    void hit(Body* part, phys_t impulse);
    void leftKick(bool state);
    void leftPunch(bool state);
    void jump(bool state);
    void moveLeft(double vel);
    void moveRight(double vel);
    void rightKick(bool state);
    void rightPunch(bool state);
    /** Update power meters. */
    void update(double deltaTime);
private:
    bool crouch_, dead_, fire_, jump_, leftKick_, leftPunch_, rightKick_,
            rightPunch_;
    Circle<phys_t> shapeBody_, shapeHead_, shapeFoot_, shapeHand_;
    CharacterBody body_;
    double powerCrouch_, powerFire_, powerJump_, powerLeftKick_, powerLeftPunch_,
            powerRightKick_, powerRightPunch_, vel_, velLeft_, velRight_;
    FixtureSpring neck_, legBack_, legFront_, armBack_, armFront_;
    static int collisionGroup_;
    SmallBody head_, footBack_, footFront_, handBack_, handFront_;
    state2p getStateAt(vector2p p);
    friend class CharacterGraphic;
};

class CharacterGraphic: public StackGraphic {
public:
    CharacterGraphic(Character* c);
private:
    Character* c_;
    GraphicFixture bodyFixture_, headFixture_, footBackFixture_,
            footFrontFixture_, handBackFixture_, handFrontFixture_;
    ColorModifier bodyColor_;
    float colour_[3];
    SizeModifier scaler_;
    TestDisk body_, head_, footBack_, footFront_, handBack_, handFront_;
};

#endif /* CHARACTER_H_ */
