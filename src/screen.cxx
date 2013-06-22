/*
 * Copyright (C) 2011, 2013 Alexander Berntsen <alexander@plaimi.net>
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
#include "game_graphics_gl.hxx"

bool Screen::_fullscreen_ = false;
int Screen::_surfaceWidth_ = 640;
int Screen::_surfaceHeight_ = 480;
int Screen::_screenWidth_ = 0;
int Screen::_screenHeight_ = 0;
int Screen::_depth_ = 0;
SDL_Surface* Screen::_surface_ = NULL;
Screen* Screen::_instance_ = NULL;

Screen* Screen::getInstance() {
    if (!_instance_ && initialize())
        _instance_ = new Screen();
    return _instance_;
}

bool Screen::getFullscreen() {
    return _fullscreen_;
}

int Screen::getSurfaceWidth() {
    return _surfaceWidth_;
}

int Screen::getSurfaceHeight() {
    return _surfaceHeight_;
}

int Screen::getDepth() {
    return _depth_;
}

void Screen::setVideoMode(int width, int height, int depth, bool fullscreen) {
    _surfaceWidth_ = width ? width : _screenWidth_;
    _surfaceHeight_ = height ? height : _screenHeight_;
    _depth_ = depth;
    _fullscreen_ = fullscreen;
    if (_surface_)
        updateVideoMode();
}

void Screen::updateVideoMode() {
    _surface_ = SDL_SetVideoMode(_surfaceWidth_, _surfaceHeight_, _depth_,
            SDL_OPENGL | (_fullscreen_ ? SDL_FULLSCREEN : SDL_RESIZABLE));
    if (!_surface_)
        printf("\nERROR: SDL_SetVideoMode failed, did you forget\n"
                "to compile SDL with OpenGL Support?\n\n");
    initGl();
}

GLfloat Screen::getGlWidth() {
    return 1.0 * _surfaceWidth_ / _surfaceHeight_;
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
    _screenWidth_ = screenSize->current_w;
    _screenHeight_ = screenSize->current_h;
    updateVideoMode();
    if (_surface_ == NULL)
        return false;
    SDL_WM_SetCaption("Limbs Off", 0);
    return glGetError() == GL_NO_ERROR;
}

void Screen::initGl() {
    glViewport(0, 0, _surfaceWidth_, _surfaceHeight_);
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
    bool pm = drawingMode_ & _DM_PREMUL;
    if (drawingMode_ & _DM_FRONT_TO_BACK) {
        s = pm ? GL_ONE_MINUS_DST_ALPHA : GL_SRC_ALPHA_SATURATE;
        d = GL_ONE;
    } else {
        s = pm ? GL_ONE : GL_SRC_ALPHA;
        d = GL_ONE_MINUS_SRC_ALPHA;
    }
    glBlendFunc(s, d);
    if ((drawingMode_ & (_DM_SMOOTH | _DM_PREMUL)) == _DM_SMOOTH)
        glEnable(GL_POLYGON_SMOOTH);
    else
        glDisable(GL_POLYGON_SMOOTH);
}

bool Screen::handle(const SDL_Event& event) {
    if (event.type == SDL_VIDEORESIZE) {
        _surfaceWidth_ = event.resize.w;
        _surfaceHeight_ = event.resize.h;
        updateVideoMode();
        return true;
    }
    return false;
}
