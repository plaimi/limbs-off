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

#include <stdio.h>
#include <math.h>
#include "game_graphics_gl.h"

Camera::Camera(vector2p position, double radius, double rotation) :
    position_(position), targetPosition_(position), radius_(radius),
        targetRadius_(radius), rotation_(rotation), targetRotation_(rotation) {
}

vector2p Camera::getPosition() {
    return position_;
}

double Camera::getRadius() {
    return radius_;
}

double Camera::getRotation() {
    return rotation_;
}
void Camera::setTargetPosition(vector2p target) {
    targetPosition_ = target;
}

void Camera::setTargetRadius(double target) {
    targetRadius_ = target;
}

void Camera::setTargetRotation(double target) {
    targetRotation_ = target;
}

void Camera::update(double deltaTime) {
    double decay = pow(.25, deltaTime);
    position_ = targetPosition_ * (1.0 - decay) + position_ * decay;
    radius_ = targetRadius_ * (1.0 - decay) + radius_ * decay;
    double diff = remainder((targetRotation_ - rotation_), 360.0);
    rotation_ += (1.0 - decay) * diff;
}

void Camera::apply() {
    glRotated(rotation_, 0.0, 0.0, -1.0);
    glScaled(1.0 / radius_, 1.0 / radius_, 0.0);
    glTranslated(-position_.x, -position_.y, 0.0);
}

void Camera::begin() {
    glPushMatrix();
    apply();
}

void Camera::end() {
    glPopMatrix();
}
