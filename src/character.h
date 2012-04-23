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

#include "physics/game_physics.h"
#include "graphics/game_graphics_gl.h"

class Character {
public:
    class CharacterBody: public SmallBody {
    public:
        CharacterBody(Character* parent, state2p state, phys_t mass,
                phys_t orientation, phys_t angVel, phys_t inertiaMoment,
                Shape<phys_t>* shape, int collisionGroup);
    protected:
        Character* parent_;
        phys_t walkCycle_;
        bool interact(AstroBody* b, double dt, vector2p& p, vector2p& im);
    };
    Character(state2p state, phys_t orientation);
    void addToUniverse(GameUniverse* u);
    /** Return current velocity (_vel). */
    double getVel();
    state2p getState();
    /** Procedures for setting request states. */
    void crouch(bool state);
    void fire(bool state);
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
    static int collisionGroup_;
    Circle<phys_t> shapeBody_, shapeHead_, shapeFoot_, shapeHand_;
    CharacterBody body_;
    SmallBody head_, footBack_, footFront_, handBack_, handFront_;
    FixtureSpring neck_, legBack_, legFront_, armBack_, armFront_;
    /** Requests. */
    bool crouch_, fire_, jump_, leftKick_, leftPunch_, rightKick_, rightPunch_;
    /** Power meters. */
    double powerFire_, powerJump_, powerLeftKick_, powerLeftPunch_,
            powerRightKick_, powerRightPunch_;
    /** Velocity and direction. */
    double vel_, velLeft_, velRight_;
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
    TestDisk body_, head_, footBack_, footFront_, handBack_, handFront_;
};

#endif /* CHARACTER_H_ */
