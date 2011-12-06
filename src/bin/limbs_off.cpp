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
#include "init.h"
#include "input_handler.h"
#include "player.h"

const phys_t GM = 628;
const phys_t R = 9.0;
const phys_t S = sqrt<phys_t> (GM / R) * 0.5;
const phys_t PR = 7.0;

int main(int argc, char *argv[]) {
    Screen* s = Screen::getInstance();
    s->setDrawingMode(Screen::DM_FRONT_TO_BACK | Screen::DM_SMOOTH, -1, false);
    TextureLoader* l = TextureLoader::getInstance();
    GLuint t = l->loadTexture("test.png", true);
    Circle<phys_t> pc = Circle<phys_t> (PR);
    AstroBody p(GM, 2 * GM * PR * PR / 5, -0.05, &pc);
    Circle<phys_t> cc = Circle<phys_t> (1.0);
    Character c(50.0, state2p()(R, 0.0, 0.0, S), 2 * 50.0 * 1 * 1 / 5, 0.0,
            -5.0, &cc);
    Player player(&c);
    GameUniverse u(&p, &c);
    Disk pd(PR, 64);
    Disk pdr(PR, 4);
    Sprite cs(t, 1, 1);
    BodyGraphic pg(&p, &pd, 0.0, 0.0, 0.0), pgr(&p, &pdr, 0.0, 0.0, 0.0), cg(
            &c, &cs, 0.0, 0.0, 0.0);
    SDL_Event event;
    Init::readBindings(&player, "src/controllers.conf");
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    ;
                }
                // Fall through
            case SDL_KEYUP:
                if (InputHandler* handler = InputHandler::getHandler(event.key.keysym.sym)) {
                    handler->handle(event);
                }
                break;
            case SDL_QUIT:
                quit = true;
            }
        }
        for (int i = 0; i < 10; i++)
            u.update(1.0 / 600.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        cg.draw();
        glColor3f(0.8, 0.4, 0.4);
        pgr.draw();
        glColor3f(0.4, 0.8, 0.4);
        pg.draw();
        SDL_GL_SwapBuffers();
    }
    return 0;
}
