/*
 * Copyright (C) 2011, 2012 Alexander Berntsen <alexander@plaimi.net>
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
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

#ifndef GAME_GRAPHICS_GL_H_
#define GAME_GRAPHICS_GL_H_

#include <vector>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "graphics.h"
#include "menu.h"
#include "physics.h"

// We should make prototypes for all the classes or solve the problems that
// cause the need for them in the first place. This looks a bit silly Lulzy
// McQuickfix (tm).
class Camera;
class GuiGraphic;

class GraphicFixture: public GraphicModifier {
public:
    GraphicFixture(Body* body);
    void begin();
    void end();
private:
    Body* body_;
};

class ColorModifier: public GraphicModifier {
public:
    ColorModifier(const float* color);
    void begin();
    void end();
private:
    const float* color_;
};

class BackgroundModifier: public GraphicModifier {
public:
    BackgroundModifier(Camera* camera);
    void begin();
    void end();
private:
    Camera* camera_;
};

class PositionModifier: public GraphicModifier {
public:
    PositionModifier(int position, int num);
    void begin();
    void end();
private:
    int position_, num_;
};

class StackGraphic: public Graphic {
public:
    /** Add a graphic to the top of the stack. */
    void addGraphic(Graphic* g);
    void doDraw();
private:
    std::vector<Graphic*> graphics_;
};

class Sprite: public Graphic {
public:
    Sprite(GLuint texture, GLfloat w, GLfloat h);
    void doDraw();
private:
    GLuint texture_;
    GLfloat w_, h_;
};

class GuiGraphic: public Graphic {
public:
    double getHeight();
    double getWidth();
    GuiElement* getLogic();
    void setSize(double w, double h);
protected:
    double width_, height_;
    /** The logic of the graphic. */
    GuiElement* logic_;
};

class Label: public Graphic {
public:
    Label(const char* face, const char* text, int size, double width, 
            double height);
    ~Label();
    char* getText();
    double getHeight();
    double getWidth();
    void doDraw();
    void setFace(const char* face);
    void setSize(int size);
    void setText(const char* text);
private:
    void make();
    /** The font name. */
    char* face_;
    /** The actual text printed. */
    char* text_;
    /** The size of the label. */
    double width_, height_;
    /* The texture created from surface_. */
    GLuint texture_;
    /** The font size. */
    int size_;
    /** The font colour. */
    SDL_Color colour_;
    /** The surface the text is rendered to. */
    SDL_Surface* surface_;
    /** The actual TTF_Font. */
    TTF_Font* font_;
};

class Disk: public Graphic {
public:
    Disk(GLfloat r, int n);
    void doDraw();
private:
    GLfloat r_;
    int n_;
    GLuint displayList_;
    void makeDisplayList();
};

class TestDisk: public StackGraphic {
public:
    TestDisk(GLfloat r, int n);
    Disk* getDisk();
    Disk* getSquare();
private:
    Disk disk_, square_;
};

class ButtonGraphic: public GuiGraphic {
public:
    ButtonGraphic(double width, double height, GuiElement* logic, 
            Label* label = NULL, bool selected = false);
    ~ButtonGraphic();
    /** The button's label if any. */
    Label* const label;
    void doDraw();
private:
};

class SubmenuGraphic: public StackGraphic {
public:
    SubmenuGraphic(Submenu* submenu);
    ~SubmenuGraphic();
    /** Add a button to buttonGraphics_ and to graphics_. */
    void addButton(GuiElement* logic, Label* label = NULL, 
            bool selected = false);
private:
    /** The logic. */
    Submenu* submenu_;
    /** The buttons to draw. */
    std::vector<ButtonGraphic*> buttonGraphics_;
    /** The position modifiers for the buttons. */
    std::vector<PositionModifier*> positionModifiers_;
};

class MenuGraphic: public Graphic {
public:
    MenuGraphic(Menu* menu);
    ~MenuGraphic();
    void doDraw();
private:
    /** The logic. */
    Menu* menu_;
    /** The menus to draw. */
    SubmenuGraphic* menuGraphics_[Menu::NUM_MENU];
    /** The labels. */
    std::vector<Label*> labels_;
};

class TextureLoader {
public:
    static TextureLoader* getInstance();
    GLuint loadTexture(const char* filename, bool premultiply);
protected:
    TextureLoader();
private:
    static TextureLoader* instance_;
};

class Screen: EventHandler {
public:
    static const int DM_FRONT_TO_BACK = 1, DM_PREMUL = 2, DM_SMOOTH = 4;
    /** Create or return a singleton. */
    static Screen* getInstance();
    static void setVideoMode(int width, int height, int depth,
            bool fullscreen = false);
    static bool getFullscreen();
    static int getSurfaceWidth();
    static int getSurfaceHeight();
    static int getDepth();
    static double getGlWidth();
    static double getGlHeight();
    static void updateVideoMode();
    void setDrawingMode(int mode, int mask = -1, bool update = true);
    int getDrawingMode();
    void updateDrawingMode();
    bool handle(const SDL_Event& event);
protected:
    Screen();
    static void initGl();
private:
    /** Video mode. */
    static bool fullscreen_;
    /** Screen & window dimensions, & depth info. */
    static int screenWidth_, screenHeight_, depth_, surfaceWidth_,
            surfaceHeight_;
    /** The singleton. */
    static Screen* instance_;
    /** The surface pointer for SetVideoMode. */
    static SDL_Surface* surface_;
    int drawingMode_;
    static bool initialize();
};

class Camera: public GraphicModifier {
public:
    Camera(state2p state, double radius, double rotation);
    /** Update pos/rad/rot. */
    void update(double deltaTime);
    state2p getState();
    double getRadius();
    double getRotation();
    void setTargetState(state2p target);
    void setTargetRadius(double target);
    void setTargetRotation(double target, double speed = 1);
    /** Rotate and translate OpenGL matrix. */
    void apply();
    void begin();
    void end();
private:
    /** Pos and desired pos. */
    state2p state_, targetState_;
    /** Rad and desired rad. */
    double radius_, targetRadius_;
    /** Rot and desired rot. */
    double rotation_, targetRotation_, rotationSpeed_;
};

#endif /* GAME_GRAPHICS_GL_H_ */
