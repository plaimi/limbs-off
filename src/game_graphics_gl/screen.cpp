/*
 * Copyright (C) 2011 Stian Ellingsen <stiell@stiell.org>
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

#include <SDL/SDL.h>
#include "game_graphics_gl.h"

Screen* Screen::instance_ = NULL;
int Screen::screenWidth_ = 640;
int Screen::screenHeight_ = 480;
int Screen::screenDepth_ = 0;

Screen* Screen::getInstance() {
    if (!instance_ && initialize())
        instance_ = new Screen();
    return instance_;
}

void Screen::setVideoMode(int screenWidth, int screenHeight, int screenDepth) {
    screenWidth_ = screenWidth;
    screenHeight_ = screenHeight;
    screenDepth_ = screenDepth;
}

bool Screen::initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;
    atexit(SDL_Quit);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    if (SDL_SetVideoMode(screenWidth_, screenHeight_, screenDepth_, SDL_OPENGL)
            == NULL)
        return false;
    SDL_WM_SetCaption("Limbs Off", 0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float R = 1.0;
    const float W = R * screenWidth_ / screenHeight_;
    glOrtho(-W, W, -R, R, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_BLEND);
    return glGetError() == GL_NO_ERROR;
}

Screen::Screen() :
    drawingMode_(0) {
}

void Screen::setDrawingMode(int mode, int mask, bool update) {
    drawingMode_ ^= (drawingMode_ ^ mode) & mask;
    if (update)
        updateDrawingMode();
}

int Screen::getDrawingMode() {
    return drawingMode_;
}

void Screen::updateDrawingMode() {
    GLenum s, d;
    bool pm = drawingMode_ & DM_PREMUL;
    if (drawingMode_ & DM_FRONT_TO_BACK) {
        s = pm ? GL_ONE_MINUS_DST_ALPHA : GL_SRC_ALPHA_SATURATE;
        d = GL_ONE;
    } else {
        s = pm ? GL_ONE : GL_SRC_ALPHA;
        d = GL_ONE_MINUS_SRC_ALPHA;
    }
    glBlendFunc(s, d);
    if ((drawingMode_ & (DM_SMOOTH | DM_PREMUL)) == DM_SMOOTH)
        glEnable(GL_POLYGON_SMOOTH);
    else
        glDisable(GL_POLYGON_SMOOTH);
}
