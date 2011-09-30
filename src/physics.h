/*
 * Copyright (C) 2011 Stian Ellingsen <stiell@stiell.org>
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

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "geometry.h"

template<typename T>
struct state2 {
    vector2<T> p, v;
    vector2<T> &operator[](unsigned int i) {
        return *(&p + i);
    }
    const vector2<T> &operator[](unsigned int i) const {
        return *(&p + i);
    }
    const state2<T> operator()(const vector2<T> p0, const vector2<T> v0) {
        p = p0;
        v = v0;
        return *this;
    }
    const state2<T> operator-(void) const {
        return state2<T> ()(-p, -v);
    }
    const state2<T> operator+(const state2<T> &s) const {
        return state2<T> ()(p + s.p, v + s.v);
    }
    const state2<T> operator-(const state2<T> &s) const {
        return state2<T> ()(p - s.p, v - s.v);
    }
    const state2<T> operator*(const T f) const {
        return state2<T> ()(p * f, v * f);
    }
    const state2<T> operator/(const T d) const {
        return state2<T> ()(p / d, v / d);
    }
    const state2<T> &operator+=(const state2<T> &s) {
        (*this)(p + s.p, v + s.v);
        return *this;
    }
    const state2<T> &operator-=(const state2<T> &s) {
        (*this)(p - s.p, v - s.v);
        return *this;
    }
    const state2<T> &operator*=(const T f) {
        (*this)(p * f, v * f);
        return *this;
    }
    const state2<T> &operator/=(const T d) {
        (*this)(p / d, v / d);
        return *this;
    }
};

typedef state2<float> state2f;

class Particle {
public:
    state2f getState();
    vector2f getPosition();
    void getPosition(float& x, float& y);
    vector2f getVelocity();
    void getVelocity(float& vx, float& vy);
    virtual ~Particle();
protected:
    union {
        state2f s_;
        struct {
            vector2f p_, v_;
        };
        struct {
            float x_, y_, vx_, vy_;
        };
    };
};

class Mass: public Particle {
public:
    float getMass();
    virtual ~Mass();
protected:
    float mass_;
};

class Body: public Mass {
public:
    float getOrientation();
    float getAngularVelocity();
    virtual ~Body();
protected:
    float orientation_;
    float av_;
};

#endif /* PHYSICS_H_ */
