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

#include <math.h>
#include <fontconfig/fontconfig.h>
#include "graphics/game_graphics_gl.h"
#include "geometry.h"

GraphicFixture::GraphicFixture(Body* body) :
    body_(body) {
}

void GraphicFixture::begin() {
    phys_t x, y;
    body_->getPosition(x, y);
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(body_->getOrientation() * IN_DEG, 0.0, 0.0, 1.0);
}

void GraphicFixture::end() {
    glPopMatrix();
}

ColorModifier::ColorModifier(const float* color) :
    color_(color) {
}

void ColorModifier::begin() {
    glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
    glColor3fv(color_);
}

void ColorModifier::end() {
    glPopAttrib();
}

BackgroundModifier::BackgroundModifier(Camera* camera) :
    camera_(camera) {
}

void BackgroundModifier::begin() {
    Screen* s = Screen::getInstance();
    double w = s->getGlWidth(), h = s->getGlHeight();
    double r = sqrt(w * w + h * h);
    glPushMatrix();
    glScaled(r, r, 1.0);
    glRotated(camera_->getRotation(), 0.0, 0.0, -1.0);
}

void BackgroundModifier::end() {
    glPopMatrix();
}

void StackGraphic::addGraphic(Graphic* g) {
    graphics_.push_back(g);
}

void StackGraphic::doDraw() {
    // Draw all the graphics in the stack in the correct order.
    if (Screen::getInstance()->getDrawingMode() & Screen::DM_FRONT_TO_BACK) {
        std::vector<Graphic*>::reverse_iterator i;
        for (i = graphics_.rbegin(); i < graphics_.rend(); i++)
            (*i)->draw();
    } else {
        std::vector<Graphic*>::iterator i;
        for (i = graphics_.begin(); i < graphics_.end(); i++)
            (*i)->draw();
    }
}

Sprite::Sprite(GLuint texture, GLfloat w, GLfloat h) :
    texture_(texture), w_(w), h_(h) {
}

void Sprite::doDraw() {
    Screen::getInstance()->setDrawingMode(-1, Screen::DM_PREMUL);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(-w_, -h_);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(w_, -h_);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(w_, h_);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(-w_, h_);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

Disk::Disk(GLfloat r, int n) :
    r_(r), n_(n), displayList_(0) {
}

void Disk::doDraw() {
    Screen::getInstance()->setDrawingMode(0, Screen::DM_PREMUL);
    if (displayList_ == 0)
        makeDisplayList();
    else
        glCallList(displayList_);
}

void Disk::makeDisplayList() {
    float t = 2 * PI / n_;
    float c = cosf(t), s = sinf(t), x = 0.0, y = r_;
    displayList_ = glGenLists(1);
    glNewList(displayList_, GL_COMPILE_AND_EXECUTE);
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex2f(x, y);
    for (int i = 1; i < n_; i++) {
        if (i & 1) {
            float z = x;
            x = c * x - s * y;
            y = s * z + c * y;
        }
        glVertex2f(i & 1 ? x : -x, y);
    }
    glEnd();
    glEndList();
}

TestDisk::TestDisk(GLfloat r, int n) :
    disk_(r, n), square_(r, 4) {
    addGraphic(&disk_);
    addGraphic(&square_);
}

Disk* TestDisk::getDisk() {
    return &disk_;
}

Disk* TestDisk::getSquare() {
    return &square_;
}
