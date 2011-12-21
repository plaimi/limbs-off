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

#ifndef PHYSICS_INL_H_
#define PHYSICS_INL_H_

#include "physics.h"

template<typename T>
inline const state1<T> state1<T>::operator()(const T p0, const T v0) {
    p = p0;
    v = v0;
    return *this;
}

template<typename T>
inline const state1<T> state1<T>::operator-() const {
    return state1<T> ()(-p, -v);
}

template<typename T>
inline const state1<T> state1<T>::operator+(const state1<T> &s) const {
    return state1<T> ()(p + s.p, v + s.v);
}

template<typename T>
inline const state1<T> state1<T>::operator-(const state1<T> &s) const {
    return state1<T> ()(p - s.p, v - s.v);
}

template<typename T>
inline const state1<T> state1<T>::operator*(const T f) const {
    return state1<T> ()(p * f, v * f);
}

template<typename T>
inline const state1<T> state1<T>::operator/(const T d) const {
    return state1<T> ()(p / d, v / d);
}

template<typename T>
inline vector2<T>& state2<T>::operator[](unsigned int i) {
    return *(&p + i);
}

template<typename T>
inline const vector2<T>& state2<T>::operator[](unsigned int i) const {
    return *(&p + i);
}

template<typename T>
inline const state2<T> state2<T>::operator()(const vector2<T> p0,
        const vector2<T> v0) {
    p = p0;
    v = v0;
    return *this;
}

template<typename T>
inline const state2<T> state2<T>::operator()(const T x, const T y, const T vx,
        const T vy) {
    p(x, y);
    v(vx, vy);
    return *this;
}

template<typename T>
inline const state2<T> state2<T>::operator-() const {
    return state2<T> ()(-p, -v);
}

template<typename T>
inline const state2<T> state2<T>::operator+(const state2<T>& s) const {
    return state2<T> ()(p + s.p, v + s.v);
}

template<typename T>
inline const state2<T> state2<T>::operator-(const state2<T>& s) const {
    return state2<T> ()(p - s.p, v - s.v);
}

template<typename T>
inline const state2<T> state2<T>::operator*(const T f) const {
    return state2<T> ()(p * f, v * f);
}

template<typename T>
inline const state2<T> state2<T>::operator/(const T d) const {
    return state2<T> ()(p / d, v / d);
}

template<typename T>
inline const state2<T>& state2<T>::operator+=(const state2<T>& s) {
    (*this)(p + s.p, v + s.v);
    return *this;
}

template<typename T>
inline const state2<T>& state2<T>::operator-=(const state2<T>& s) {
    (*this)(p - s.p, v - s.v);
    return *this;
}

template<typename T>
inline const state2<T>& state2<T>::operator*=(const T f) {
    (*this)(p * f, v * f);
    return *this;
}

template<typename T>
inline const state2<T>& state2<T>::operator/=(const T d) {
    (*this)(p / d, v / d);
    return *this;
}

inline Particle::Particle(state2p s) :
    s_(s) {
}

inline Particle::~Particle() {
}

inline state2p Particle::getState() {
    return s_;
}

inline vector2p Particle::getPosition() {
    return s_.p;
}

inline void Particle::getPosition(phys_t& x, phys_t& y) {
    x = s_.p.x;
    y = s_.p.y;
}

inline vector2p Particle::getVelocity() {
    return s_.v;
}

inline void Particle::getVelocity(phys_t& vx, phys_t& vy) {
    vx = s_.v.x;
    vy = s_.v.y;
}

inline Mass::Mass(state2p s, phys_t mass) :
    Particle(s), mass_(mass) {
}

inline Mass::~Mass() {
}

inline phys_t Mass::getMass() {
    return mass_;
}

inline vector2p Mass::getMomentum() {
    return s_.v * mass_;
}

inline void Mass::applyImpulse(vector2p i) {
    s_.v += i / mass_;
}

inline Body::Body(state2p s, phys_t mass, phys_t orientation, phys_t av,
        phys_t moi, Shape<phys_t>* shape) :
    Mass(s, mass), orientation_(orientation), av_(av), moi_(moi), shape_(shape) {
}

inline Body::~Body() {
}

inline phys_t Body::getOrientation() {
    return orientation_;
}

inline phys_t Body::getAngularVelocity() {
    return av_;
}

inline phys_t Body::getMomentOfInertia() {
    return moi_;
}

inline Shape<phys_t>* Body::getShape() {
    return shape_;
}

inline phys_t Body::getAngularMomentum() {
    return av_ * moi_;
}

inline vector2p Body::getMomentumAt(vector2p p, vector2p vp) {
    phys_t pp = p.squared();
    vector2p m = (s_.v + ~p * av_ - vp) * mass_;
    m -= ~p * (~p * m * mass_ / (moi_ + mass_ * pp));
    return m;
}

inline vector2p Body::getVelocityAt(vector2p p) {
    return s_.v + ~p * av_;
}

inline void Body::applyAngularImpulse(phys_t i) {
    av_ += i / moi_;
}

inline void Body::applyImpulseAt(vector2p i, vector2p p) {
    applyImpulse(i);
    applyAngularImpulse(p / i);
}

inline phys_t momentInertia(phys_t mass, phys_t radius, phys_t dist) {
    return radius * radius * dist * mass;
}

#endif /* PHYSICS_INL_H_ */
