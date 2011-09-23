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

#include "game_graphics_gl.h"

BodySprite::BodySprite(Body* body, GLuint texture, GLfloat w, GLfloat h,
        GLfloat ox, GLfloat oy, GLfloat oa) :
    body_(body), texture_(texture), w_(w), h_(h), ox_(ox), oy_(oy), oa_(oa) {
}

void BodySprite::draw() {
    float x, y;
    body_->getPosition(x, y);
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(body_->getOrientation() - oa_, 0.0, 0.0, 1.0);
    glTranslatef(-ox_, -oy_, 0.0);
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
