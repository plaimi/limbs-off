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

#ifndef TEMPLATE_MATH_INL_H_
#define TEMPLATE_MATH_INL_H_

#include "template_math.h"

template<>
inline float sqrt<float> (float x) {
    return sqrtf(x);
}

template<>
inline double sqrt<double> (double x) {
    return sqrt(x);
}

template<>
inline float sin<float> (float x) {
    return sinf(x);
}

template<>
inline double sin<double> (double x) {
    return sin(x);
}

template<>
inline float cos<float> (float x) {
    return cosf(x);
}

template<>
inline double cos<double> (double x) {
    return cos(x);
}

template<typename T>
inline void sincos(T x, T& sinx, T& cosx) {
    sinx = sin<T> (x);
    cosx = cos<T> (x);
}

#ifdef __USE_GNU

template<>
inline void sincos<float> (float x, float& sinx, float& cosx) {
    sincosf(x, &sinx, &cosx);
}

template<>
inline void sincos<double> (double x, double& sinx, double& cosx) {
    sincos(x, &sinx, &cosx);
}

#endif

template<>
inline float atan2(float y, float x) {
    return atan2f(y, x);
}

template<>
inline double atan2(double y, double x) {
    return atan2(y, x);
}

template<typename T>
inline T abs(T x) {
    return x < 0 ? -x : x;
}

template<>
inline float abs(float x) {
    return fabsf(x);
}

template<>
inline double abs(double x) {
    return fabs(x);
}

template<>
inline float remainder<float> (float numerator, float denominator) {
    return remainderf(numerator, denominator);
}

template<>
inline double remainder<double> (double numerator, double denominator) {
    return remainder(numerator, denominator);
}

template<>
inline float copysign<float> (float x, float y) {
    return copysignf(x, y);
}

template<>
inline double copysign<double> (double x, double y) {
    return copysign(x, y);
}

template<typename T>
T min(T x, T y) {
    return std::min<T>(x, y);
}

template<>
inline float min<float> (float x, float y) {
    return fminf(x, y);
}

template<>
inline double min<double> (double x, double y) {
    return fmin(x, y);
}

template<typename T>
inline T max(T x, T y) {
    return std::max<T>(x, y);
}

template<>
inline float max<float> (float x, float y) {
    return fmaxf(x, y);
}

template<>
inline double max<double> (double x, double y) {
    return fmax(x, y);
}

template<typename T>
inline T clampmag(T x, T m) {
    return max<T> (-m, min<T> (m, x));
}

#endif /* TEMPLATE_MATH_INL_H_ */
