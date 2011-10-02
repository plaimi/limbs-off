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
#include "game_physics.h"

const phys_t GM = 480.5;
const phys_t R = 9.0;
const phys_t S = sqrtp(GM / R);

int main(int argc, char *argv[]) {
    Screen* s = Screen::getInstance();
    TextureLoader* l = TextureLoader::getInstance();
    GLuint t = l->loadTexture("test.png", true);
    AstroBody p(GM, 0.0);
    Character c(50.0, state2p()(R, 0.0, 0.0, S), 0.0, 2.0 * S / R);
    Universe u(&p, &c);
    Disk pd(7.0, 64);
    Sprite cs(t, 1.0, 1.0);
    BodyGraphic pg(&p, &pd, 0.0, 0.0, 0.0), cg(&c, &cs, 0.0, 0.0, 0.0);
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
        for (int i = 0; i < 10; i++)
            u.update(1.0 / 600.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.4, 0.8, 0.4);
        pg.draw();
        glColor3f(1.0, 1.0, 1.0);
        cg.draw();
        SDL_GL_SwapBuffers();
    }
    return 0;
}
