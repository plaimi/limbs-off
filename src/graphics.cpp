#include "graphics.h"

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
