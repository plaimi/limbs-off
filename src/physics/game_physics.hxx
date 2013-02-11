/*
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
 * Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
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

#ifndef GAME_PHYSICS_HXX_
#define GAME_PHYSICS_HXX_

#include <vector>
#include "physics.hxx"

class Character;
class Universe;

class SmallBody: public Body {
public:
    SmallBody(state2p s, phys_t mass, phys_t orientation, phys_t av,
            phys_t moi, Shape<phys_t>* shape, int collisionGroup);
protected:
    virtual bool interact(class AstroBody* bvz, double dt, vector2p& p,
            vector2p& im);
private:
    void applyImpulseAndRewind(vector2p impulse, vector2p pos, phys_t dt,
            phys_t fraction);
    void updateState(phys_t dt);
    void setDeltaState(int i, vector2p a);
    bodystate getNextState(phys_t dt);
    state2p ds_[4];
    bodystate nextState_;
    int collisionGroup_;
    friend class GameUniverse;
};

class AstroBody: public Body {
public:
    const phys_t gm;
    AstroBody(phys_t gm, phys_t moi, phys_t av, Shape<phys_t>* shape);
    friend class GameUniverse;
};

class Link {
public:
    virtual ~Link() { }
    Link(SmallBody* a, SmallBody* b);
    virtual void update(phys_t dt, class GameUniverse* u) = 0;
protected:
    SmallBody* const a_;
    SmallBody* const b_;
private:
    Link(const Link&);
    Link& operator=(const Link&);
};

class GameUniverse: public Universe {
public:
    GameUniverse(AstroBody* planet);
    void update(phys_t dt);
    void addBody(SmallBody* b);
    void addLink(Link* l);
    void applyImpulse(SmallBody* a, SmallBody* b, vector2p im, vector2p pos);
    void applyAngularImpulse(SmallBody* a, SmallBody* b, phys_t im);
private:
    GameUniverse(const GameUniverse&);
    GameUniverse& operator=(const GameUniverse&);
    AstroBody* planet_;
    std::vector<SmallBody*> smallBodies_;
    std::vector<Link*> links_;
};

class FixtureSpring: public Link {
public:
    FixtureSpring(SmallBody* a, SmallBody* b, phys_t lStiff, phys_t lDamp,
            phys_t aStiff, phys_t aDamp);
    void setEnabled(bool status);
    void setPosition(vector2p position);
    void setOrientation(phys_t orientation);
    state2p getTargetState();
    void update(phys_t dt, GameUniverse* u);
protected:
    bool enabled_;
    phys_t lStiff_, lDamp_, aStiff_, aDamp_;
    vector2p position_;
    phys_t orientation_;
};

#endif /* GAME_PHYSICS_HXX_ */
