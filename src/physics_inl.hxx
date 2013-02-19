/*
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
 * Copyright (C) 2013 Alexander Berntsen <alexander@plaimi.net>
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

#ifndef PHYSICS_INL_HXX_
#define PHYSICS_INL_HXX_

#include "physics.hxx"

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
inline const state2<T> state2<T>::operator()(const vector2<T> &p0,
        const vector2<T> &v0) {
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

inline const bodystate bodystate::operator+(const bodystate& s) const {
    bodystate r = { l + s.l, a + s.a };
    return r;
}

inline const bodystate bodystate::operator*(const phys_t f) const {
    bodystate r = { l * f, a * f };
    return r;
}

inline Material::Material(phys_t stiffness, phys_t toughness) :
        stiffness_(stiffness),
        toughness_(toughness) {
}

inline phys_t Material::getToughness() {
    return toughness_;
}

inline phys_t Material::getStiffness() {
    return stiffness_;
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

inline Mass::Mass(state2p s, phys_t mass, bool immovable) :
        Particle(s), mass_(mass), invMass_(immovable ? 0 : 1 / mass) {
}

inline Mass::~Mass() {
}

inline phys_t Mass::getMass() {
    return mass_;
}

inline phys_t Mass::getInvMass() {
    return invMass_;
}

inline vector2p Mass::getMomentum() {
    return s_.v * mass_;
}

inline void Mass::applyImpulse(vector2p i) {
    s_.v += i * invMass_;
}

inline Body::Body(state2p s, phys_t mass, phys_t orientation, phys_t av, phys_t
        moi, Shape<phys_t>* shape, Material* material, bool immovable) :
        Mass(s, mass, immovable), orientation_(orientation), av_(av), moi_(moi),
        shape_(shape), material_(material) {
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

inline Material* Body::getMaterial() {
    return material_;
}

inline void Body::setMaterial(Material* m) {
    material_ = m;
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

inline bodystate Body::getBodyState() {
    bodystate r = { s_, { orientation_, av_ } };
    return r;
}

inline void Body::applyAngularImpulse(phys_t i) {
    av_ += i / moi_;
}

inline void Body::applyImpulseAt(vector2p i, vector2p p) {
    applyImpulse(i);
    applyAngularImpulse(p / i);
}

inline void Body::setBodyState(bodystate s) {
    s_ = s.l;
    orientation_ = s.a.p;
    av_ = s.a.v;
}

inline bool Collision::operator<(const Collision& b) const {
    return time < b.time;
}

inline bool Collision::conflicts(const Collision& c) const {
    if (body[0]->getInvMass() != 0)
        return body[0] == c.body[1] || body[0] == c.body[0] ||
                body[1] == c.body[1] || body[1] == c.body[0];
    return body[1] == c.body[1];
}

inline Collision CollisionQueue::pop() {
    Collision c = collisions_.front();
    collisions_.pop_front();
    return c;
}

inline bool CollisionQueue::empty() {
    return collisions_.empty();
}

inline phys_t momentInertia(phys_t mass, phys_t radius, phys_t dist) {
    return radius * radius * dist * mass;
}

#endif /* PHYSICS_INL_HXX_ */
