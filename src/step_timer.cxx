/*
 * Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
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

#include "step_timer.hxx"

StepTimer::StepTimer() :
        time_(0.0),
        targetTime_(0.0),
        deltaTargetTime_(0.0) {
}

void StepTimer::targetTime(double dt) {
    targetTime_ += dt;
    deltaTargetTime_ = (15 * deltaTargetTime_ + dt) / 16;
}

void StepTimer::time(double dt) {
    time_ += dt;
}

double StepTimer::getStepTime() {
    return (15 * deltaTargetTime_ + targetTime_ - time_) / 16;
}
