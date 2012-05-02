/*
 * Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
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

#include "collision_handler.hxx"

CollisionHandler* CollisionHandler::instance_ = NULL;

CollisionHandler* CollisionHandler::getInstance() {
    if (!instance_)
        instance_ = new CollisionHandler();
    return instance_;
}

void CollisionHandler::collide(Body* body0, Body* body1, phys_t impulse) {
    std::map<Body*, Character*>::iterator it;
    if ((it = monitored_.find(body0)) != monitored_.end())
        it->second->hit(body0, impulse);
    if ((it = monitored_.find(body1)) != monitored_.end())
        it->second->hit(body1, impulse);
}

void CollisionHandler::monitor(Body* body, Character* character) {
    monitored_.insert(std::pair<Body*, Character*>(body, character));
}

