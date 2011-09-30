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

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#define PI (3.141592653589793)
#define IN_DEG (180.0 / PI)

template<typename T>
struct vector2 {
    T x, y;
    T &operator[](unsigned int i) {
        return *(&x + i);
    }
    const T &operator[](unsigned int i) const {
        return *(&x + i);
    }
    const vector2<T> operator()(const T x0, const T y0) {
        x = x0;
        y = y0;
        return *this;
    }
    const vector2<T> operator-(void) const {
        return vector2<T> ()(-x, -y);
    }
    const vector2<T> operator+(const vector2<T> &v) const {
        return vector2<T> ()(x + v.x, y + v.y);
    }
    const vector2<T> operator-(const vector2<T> &v) const {
        return vector2<T> ()(x - v.x, y - v.y);
    }
    const vector2<T> operator*(const T f) const {
        return vector2<T> ()(x * f, y * f);
    }
    const vector2<T> operator/(const T d) const {
        return vector2<T> ()(x / d, y / d);
    }
    const vector2<T> &operator+=(const vector2<T> &v) {
        (*this)(x + v.x, y + v.y);
        return *this;
    }
    const vector2<T> &operator-=(const vector2<T> &v) {
        (*this)(x - v.x, y - v.y);
        return *this;
    }
    const vector2<T> &operator*=(const T f) {
        (*this)(x * f, y * f);
        return *this;
    }
    const vector2<T> &operator/=(const T d) {
        (*this)(x / d, y / d);
        return *this;
    }
    T operator*(const vector2<T> &v) const {
        return x * v.x + y * v.y;
    }
    T squared() const {
        return x * x + y * y;
    }
};

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

typedef vector2<float> vector2f;
typedef state2<float> state2f;

#endif /* GEOMETRY_H_ */
