/*
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
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

#ifndef PHYSICS_HXX_
#define PHYSICS_HXX_

#include <math.h>
#include <list>
#include "geometry.hxx"

typedef double phys_t;

const float G = 6.67384e-11;

template<typename T>
struct state1 {
    T p, v;
    const state1<T> operator()(const T p0, const T v0);
    const state1<T> operator-() const;
    const state1<T> operator+(const state1<T> &s) const;
    const state1<T> operator-(const state1<T> &s) const;
    const state1<T> operator*(const T f) const;
    const state1<T> operator/(const T d) const;
};

template<typename T>
struct state2 {
    vector2<T> p, v;
    vector2<T>& operator[](unsigned int i);
    const vector2<T>& operator[](unsigned int i) const;
    const state2<T> operator()(const vector2<T> &p0, const vector2<T> &v0);
    const state2<T> operator()(const T x, const T y, const T vx, const T vy);
    const state2<T> operator-() const;
    const state2<T> operator+(const state2<T>& s) const;
    const state2<T> operator-(const state2<T>& s) const;
    const state2<T> operator*(const T f) const;
    const state2<T> operator/(const T d) const;
    const state2<T>& operator+=(const state2<T>& s);
    const state2<T>& operator-=(const state2<T>& s);
    const state2<T>& operator*=(const T f);
    const state2<T>& operator/=(const T d);
};

typedef vector2<phys_t> vector2p;
typedef state1<phys_t> state1p;
typedef state2<phys_t> state2p;

struct bodystate {
    state2p l;
    state1p a;
    const bodystate operator+(const bodystate& s) const;
    const bodystate operator*(const phys_t f) const;
};

class Particle {
public:
    state2p getState();
    vector2p getPosition();
    void getPosition(phys_t& x, phys_t& y);
    vector2p getVelocity();
    void getVelocity(phys_t& vx, phys_t& vy);
    virtual ~Particle();
protected:
    state2p s_;
    Particle(state2p s);
    friend class Universe;
};

class Mass: public Particle {
public:
    phys_t getMass();
    phys_t getInvMass();
    vector2p getMomentum();
    virtual ~Mass();
protected:
    phys_t mass_, invMass_;
    Mass(state2p s, phys_t mass, bool immovable = false);
    void applyImpulse(vector2p i);
    friend class Universe;
};

class Body: public Mass {
public:
    phys_t getOrientation();
    phys_t getAngularVelocity();
    phys_t getMomentOfInertia();
    Shape<phys_t>* getShape();
    phys_t getAngularMomentum();
    vector2p getMomentumAt(vector2p p, vector2p vp);
    vector2p getVelocityAt(vector2p p);
    bodystate getBodyState();
    virtual ~Body();
protected:
    phys_t orientation_;
    phys_t av_;
    phys_t moi_;
    Shape<phys_t>* shape_;
    Body(state2p s, phys_t mass, phys_t orientation, phys_t av, phys_t moi,
            Shape<phys_t>* shape, bool immovable = false);
    void applyAngularImpulse(phys_t i);
    void applyImpulseAt(vector2p i, vector2p p);
    void setBodyState(bodystate s);
    friend class Universe;
};

class Universe {
public:
    virtual void update(phys_t dt) = 0;
};

struct Collision {
public:
    phys_t time;
    Body* body[2];
    bodystate state[2];
    vector2p position, normal;
    bool operator<(const Collision& b) const;
    bool conflicts(const Collision& c) const;
};

class CollisionQueue {
public:
    void add(Collision c);
    Collision pop();
    bool empty();
private:
    std::list<Collision> collisions_;
};

phys_t momentInertia(phys_t mass, phys_t radius, phys_t dist = 1.0);

bool collide(Body* a, Body* b, bodystate& na, bodystate& nb, phys_t& t,
        vector2p& p, vector2p& n);

vector2p bounce1(Body* a, Body* b, vector2p& pa, vector2p pb, vector2p n,
        phys_t restitution, phys_t friction, phys_t rr, phys_t addVel);

vector2p bounce2(Body* a, Body* b, vector2p& pa, vector2p& pb, vector2p n,
        phys_t restitution, phys_t friction, phys_t rr, phys_t addVel);

#include "physics_inl.hxx"

#endif /* PHYSICS_HXX_ */
