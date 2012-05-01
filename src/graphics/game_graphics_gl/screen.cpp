/*
 * Copyright (C) 2011 Alexander Berntsen <alexander@plaimi.net>
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

#include <SDL/SDL.h>
#include "graphics/game_graphics_gl.h"

bool Screen::fullscreen_ = false;
int Screen::surfaceWidth_ = 640;
int Screen::surfaceHeight_ = 480;
int Screen::screenWidth_ = 0;
int Screen::screenHeight_ = 0;
int Screen::depth_ = 0;
SDL_Surface* Screen::surface_ = NULL;
Screen* Screen::instance_ = NULL;

Screen* Screen::getInstance() {
    if (!instance_ && initialize())
        instance_ = new Screen();
    return instance_;
}

bool Screen::getFullscreen() {
    return fullscreen_;
}

int Screen::getSurfaceWidth() {
    return surfaceWidth_;
}

int Screen::getSurfaceHeight() {
    return surfaceHeight_;
}

int Screen::getDepth() {
    return depth_;
}

void Screen::setVideoMode(int width, int height, int depth, bool fullscreen) {
    surfaceWidth_ = width ? width : screenWidth_;
    surfaceHeight_ = height ? height : screenHeight_;
    depth_ = depth;
    fullscreen_ = fullscreen;
    if (surface_)
        updateVideoMode();
}

void Screen::updateVideoMode() {
    surface_ = SDL_SetVideoMode(surfaceWidth_, surfaceHeight_, depth_,
            SDL_OPENGL | (fullscreen_ ? SDL_FULLSCREEN : SDL_RESIZABLE));
    initGl();
}

GLfloat Screen::getGlWidth() {
    return 1.0 * surfaceWidth_ / surfaceHeight_;
}

GLfloat Screen::getGlHeight() {
    return 1.0;
}

bool Screen::initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;
    SDL_ShowCursor(SDL_DISABLE);
    atexit(SDL_Quit);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    const SDL_VideoInfo* screenSize = SDL_GetVideoInfo();
    screenWidth_ = screenSize->current_w;
    screenHeight_ = screenSize->current_h;
    updateVideoMode();
    if (surface_ == NULL)
        return false;
    SDL_WM_SetCaption("Limbs Off", 0);
    return glGetError() == GL_NO_ERROR;
}

void Screen::initGl() {
    glViewport(0, 0, surfaceWidth_, surfaceHeight_);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat w = getGlWidth(), h = getGlHeight();
    glOrtho(-w, w, -h, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
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

bool Screen::handle(const SDL_Event& event) {
    if (event.type == SDL_VIDEORESIZE) {
        surfaceWidth_ = event.resize.w;
        surfaceHeight_ = event.resize.h;
        updateVideoMode();
        return true;
    }
    return false;
}
