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

class TestUniverse;

class TestBody: public Body {
public:
    TestBody() {
        x_ = 0.0;
        y_ = 0.0;
        orientation_ = 0;
    }
    friend class TestUniverse;
};

class TestUniverse {
public:
    TestUniverse(TestBody* body) :
        body_(body) {
    }
    void update() {
        body_->orientation_ += 1;
    }
private:
    TestBody* body_;
};

int main(int argc, char *argv[]) {
    Screen* s = Screen::getInstance();
    TextureLoader* l = TextureLoader::getInstance();
    GLuint t = l->loadTexture("test.png", true);
    TestBody b;
    TestUniverse u(&b);
    BodySprite bs(&b, t, 1.0, 1.0, 0.0, 0.0, 0.0);
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym
                    == SDLK_ESCAPE)
                quit = true;
            if (event.type == SDL_QUIT)
                quit = true;
        }
        u.update();
        glClear(GL_COLOR_BUFFER_BIT);
        bs.draw();
        SDL_GL_SwapBuffers();
    }
    return 0;
}
