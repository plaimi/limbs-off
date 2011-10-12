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
    T& operator[](unsigned int i);
    const T& operator[](unsigned int i) const;
    const vector2<T> operator()(const T x0, const T y0);
    const vector2<T> operator-() const;
    const vector2<T> operator~() const;
    const vector2<T> operator+(const vector2<T>& v) const;
    const vector2<T> operator-(const vector2<T>& v) const;
    const vector2<T> operator*(const T f) const;
    const vector2<T> operator/(const T d) const;
    const vector2<T>& operator+=(const vector2<T>& v);
    const vector2<T>& operator-=(const vector2<T>& v);
    const vector2<T>& operator*=(const T f);
    const vector2<T>& operator/=(const T d);
    T operator*(const vector2<T>& v) const;
    T operator/(const vector2<T>& v) const;
    T squared() const;
    const vector2<T> rotate(vector2<T> u);
    const vector2<T> rotated(vector2<T> u) const;
};

#include "geometry_inl.h"

#endif /* GEOMETRY_H_ */
