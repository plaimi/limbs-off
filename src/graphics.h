#ifndef GRAPHICS_H_
#define GRAPHICS_H_

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
    void beginModifiers();
    void endModifiers();
    std::vector<GraphicModifier*> modifiers_;
};

#endif /* GRAPHICS_H_ */
