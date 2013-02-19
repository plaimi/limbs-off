/*
 * Copyright (C) 2012, 2013 Alexander Berntsen <alexander@plaimi.net>
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

CollisionHandler* CollisionHandler::_instance_ = NULL;

CollisionHandler::CollisionHandler() :
        monitored_() {
}

CollisionHandler* CollisionHandler::getInstance() {
    if (!_instance_)
        _instance_ = new CollisionHandler();
    return _instance_;
}

void CollisionHandler::collide(Body* body0, Body* body1, phys_t impulse) {
    std::map<Body*, Character*>::iterator it;
    phys_t dmg = 0.001 * impulse * impulse * (body0->getInvMass() +
            body1->getInvMass()), dmg0, dmg1;
    Material* m0 = body0->getMaterial();
    Material* m1 = body1->getMaterial();
    phys_t s0 = m0->getStiffness();
    phys_t s1 = m1->getStiffness();
    dmg0 = dmg * s1 / (s0 + s1);
    dmg1 = dmg - dmg0;
    if ((it = monitored_.find(body0)) != monitored_.end())
        it->second->hit(body0, dmg0 / m0->getToughness());
    if ((it = monitored_.find(body1)) != monitored_.end())
        it->second->hit(body1, dmg1 / m1->getToughness());
}

void CollisionHandler::monitor(Body* body, Character* character) {
    monitored_.insert(std::pair<Body*, Character*>(body, character));
}

