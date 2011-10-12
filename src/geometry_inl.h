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

#ifndef GEOMETRY_INL_H_
#define GEOMETRY_INL_H_

#include "geometry.h"

template<typename T>
inline T& vector2<T>::operator[](unsigned int i) {
    return *(&x + i);
}

template<typename T>
inline const T& vector2<T>::operator[](unsigned int i) const {
    return *(&x + i);
}

template<typename T>
inline const vector2<T> vector2<T>::operator()(const T x0, const T y0) {
    x = x0;
    y = y0;
    return *this;
}

template<typename T>
inline const vector2<T> vector2<T>::operator-() const {
    return vector2<T> ()(-x, -y);
}

template<typename T>
inline const vector2<T> vector2<T>::operator~() const {
    return vector2<T> ()(-y, x);
}

template<typename T>
inline const vector2<T> vector2<T>::operator+(const vector2<T>& v) const {
    return vector2<T> ()(x + v.x, y + v.y);
}

template<typename T>
inline const vector2<T> vector2<T>::operator-(const vector2<T>& v) const {
    return vector2<T> ()(x - v.x, y - v.y);
}

template<typename T>
inline const vector2<T> vector2<T>::operator*(const T f) const {
    return vector2<T> ()(x * f, y * f);
}

template<typename T>
inline const vector2<T> vector2<T>::operator/(const T d) const {
    return vector2<T> ()(x / d, y / d);
}

template<typename T>
inline const vector2<T>& vector2<T>::operator+=(const vector2<T>& v) {
    (*this)(x + v.x, y + v.y);
    return *this;
}

template<typename T>
inline const vector2<T>& vector2<T>::operator-=(const vector2<T>& v) {
    (*this)(x - v.x, y - v.y);
    return *this;
}

template<typename T>
inline const vector2<T>& vector2<T>::operator*=(const T f) {
    (*this)(x * f, y * f);
    return *this;
}

template<typename T>
inline const vector2<T>& vector2<T>::operator/=(const T d) {
    (*this)(x / d, y / d);
    return *this;
}

template<typename T>
inline T vector2<T>::operator/(const vector2<T>& v) const {
    return ~(*this) * v;
}

template<typename T>
inline T vector2<T>::operator*(const vector2<T>& v) const {
    return x * v.x + y * v.y;
}

template<typename T>
inline T vector2<T>::squared() const {
    return x * x + y * y;
}

template<typename T>
inline const vector2<T> vector2<T>::rotate(vector2<T> u) {
    (*this)(x * u.x - y * u.y, x * u.y + y * u.x);
    return *this;
}

template<typename T>
inline const vector2<T> vector2<T>::rotated(vector2<T> u) const {
    return vector2<T> (*this).rotate(u);
}

#endif /* GEOMETRY_INL_H_ */
