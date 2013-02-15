/*
 * Copyright (C) 2011, 2013 Alexander Berntsen <alexander@plaimi.net>
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

#include <stdio.h>
#include <math.h>
#include "graphics/game_graphics_gl.hxx"

Camera::Camera(state2p state, GLfloat radius, GLfloat rotation) :
        state_(state),
        targetState_(state),
        radius_(radius),
        targetRadius_(radius),
        rotation_(rotation),
        targetRotation_(rotation),
        rotationSpeed_(0) {
}

state2p Camera::getState() {
    return state_;
}

GLfloat Camera::getRadius() {
    return radius_;
}

GLfloat Camera::getRotation() {
    return rotation_;
}
void Camera::setTargetState(state2p target) {
    targetState_ = target;
}

void Camera::setTargetRadius(GLfloat target) {
    targetRadius_ = target;
}

void Camera::setTargetRotation(GLfloat target, GLfloat speed) {
    targetRotation_ = target;
    rotationSpeed_ = speed;
}

void Camera::update(GLfloat deltaTime) {
    GLfloat decay = pow(.1, deltaTime);
    state_ = targetState_ * (1.0 - decay) + state_ * decay;
    state_.p += state_.v * deltaTime;
    radius_ = targetRadius_ * (1.0 - decay) + radius_ * decay;
    GLfloat diff = remainder((targetRotation_ - rotation_), 360.0);
    rotation_ += (1.0 - pow(.1, rotationSpeed_ * deltaTime)) * diff;
}

void Camera::apply() {
    glRotatef(rotation_, 0.0, 0.0, -1.0);
    glScalef(1.0 / radius_, 1.0 / radius_, 0.0);
    glTranslatef(-state_.p.x, -state_.p.y, 0.0);
}

void Camera::begin() {
    glPushMatrix();
    apply();
}

void Camera::end() {
    glPopMatrix();
}
