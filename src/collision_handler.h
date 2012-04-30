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

#ifndef COLLISION_HANDLER_H_
#define COLLISION_HANDLER_H_

#include <map>
#include "character.h"
#include "physics.h"

class CollisionHandler {
public:
    /** Create or return a singleton. */
    static CollisionHandler* getInstance();
    void collide(Body* body0, Body* body1, phys_t impulse);
    void monitor(Body* body, Character* character);
private:
    /** The singleton. */
    static CollisionHandler* instance_;
    std::map<Body*, Character*> monitored_;
};

#endif /*COLLISION_HANDLER_H_*/
