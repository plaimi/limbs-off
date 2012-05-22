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

#ifndef GRAPHICS_HXX_
#define GRAPHICS_HXX_

#include <vector>

/** Graphic modifier. Modifies how a graphic is drawn. */
class GraphicModifier {
public:
    virtual ~GraphicModifier();
    virtual void begin() = 0;
    virtual void end() = 0;
};

class Graphic {
public:
    virtual ~Graphic() = 0;
    /** Draws the graphic without modifiers. */
    virtual void doDraw() = 0;
    /** Draws the graphic with all its modifiers applied. */
    void draw();
    /** Appends a modifier to the list of modifiers. */
    void addModifier(GraphicModifier* modifier);
protected:
    /** Add modifiers to a graphic. */
    void beginModifiers();
    /** Remove modifiers from a graphic. */
    void endModifiers();
    std::vector<GraphicModifier*> modifiers_;
};

#endif /* GRAPHICS_HXX_ */
