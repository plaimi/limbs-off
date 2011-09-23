#ifndef GRAPHICS_H_
#define GRAPHICS_H_

class Graphic {
public:
    virtual ~Graphic() = 0;
    virtual void draw() = 0;
};

#endif /* GRAPHICS_H_ */
