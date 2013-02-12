/*
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

#ifndef GEOMETRY_HXX_
#define GEOMETRY_HXX_

#include "template_math.hxx"

#define PI (3.141592653589793)
#define IN_DEG (180.0 / PI)

template<typename T>
struct vector2 {
    T x, y;
    static vector2<T> fromAngle(T angle);
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
    T length() const;
    T angle() const;
    const vector2<T> unit() const;
    const vector2<T>& norm();
    const vector2<T> rotated(vector2<T> u) const;
    const vector2<T>& rotate(vector2<T> u);
};

enum ShapeType {
    CIRCLE, RECTANGLE
};

template<typename T>
class Shape {
public:
    virtual ~Shape() { }
    ShapeType getType() {
        return type_;
    }
protected:
    ShapeType type_;
};

template<typename T>
class Circle: public Shape<T> {
public:
    Circle(T radius) :
        radius_(radius) {
        Shape<T>::type_ = CIRCLE;
    }
    T getRadius() {
        return radius_;
    }
    void setRadius(T radius) {
        radius_ = radius;
    }
protected:
    T radius_;
};

/**
 * Calculate intersection between a line and a circle.
 *
 * @param a point on line at t=0.
 * @param b point on line at t=1.
 * @param rr squared radius of the circle.
 * @param[out] t1 t of first intersection.
 * @param[out] t2 t of second intersection.
 */
template<typename T>
bool intersectLineCircle(vector2<T> a, vector2<T> b, T rr, T& t1, T& t2);

#include "geometry_inl.hxx"

#endif /* GEOMETRY_HXX_ */
