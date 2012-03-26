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

#ifndef TEMPLATE_MATH_H_
#define TEMPLATE_MATH_H_

#include <math.h>
#include <algorithm>

template<typename T>
T sqrt(T x);

template<typename T>
T sin(T x);

template<typename T>
T cos(T x);

template<typename T>
void sincos(T x, T& sinx, T& cosx);

template<typename T>
T atan2(T y, T x);

template<typename T>
T abs(T x);

template<typename T>
T remainder(T numerator, T denominator);

template<typename T>
T copysign(T x, T y);

template<typename T>
T min(T x, T y);

template<typename T>
T max(T x, T y);

template<typename T>
T clampmag(T x, T m);

#include "template_math_inl.h"

#endif /* TEMPLATE_MATH_H_ */
