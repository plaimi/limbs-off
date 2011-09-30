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
#include "geometry.h"
#include "game_graphics_gl.h"

BodyGraphic::BodyGraphic(Body* body, Graphic* graphic, GLfloat ox, GLfloat oy,
        GLfloat oa) :
    body_(body), graphic_(graphic), ox_(ox), oy_(oy), oa_(oa) {
}

void BodyGraphic::draw() {
    float x, y;
    body_->getPosition(x, y);
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef((body_->getOrientation() - oa_) * IN_DEG, 0.0, 0.0, 1.0);
    glTranslatef(-ox_, -oy_, 0.0);
    graphic_->draw();
    glPopMatrix();
}

Sprite::Sprite(GLuint texture, GLfloat w, GLfloat h) :
    texture_(texture), w_(w), h_(h) {
}

void Sprite::draw() {
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

void Disk::draw() {
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
