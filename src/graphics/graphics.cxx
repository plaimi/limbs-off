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

#include "graphics.hxx"

GraphicModifier::~GraphicModifier() {
}

Graphic::~Graphic() {
}

void Graphic::draw() {
    beginModifiers();
    doDraw();
    endModifiers();
}

void Graphic::addModifier(GraphicModifier* modifier) {
    modifiers_.push_back(modifier);
}

void Graphic::beginModifiers() {
    // Iterate modifiers in reverse so that the last added modifier is the
    // outermost modifier.
    std::vector<GraphicModifier*>::reverse_iterator i;
    for (i = modifiers_.rbegin(); i < modifiers_.rend(); i++)
        (*i)->begin();
}

void Graphic::endModifiers() {
    // Iterate modifiers in the list order so that they are removed in the
    // reverse of the order they were applied.
    std::vector<GraphicModifier*>::iterator i;
    for (i = modifiers_.begin(); i < modifiers_.end(); i++)
        (*i)->end();
}
