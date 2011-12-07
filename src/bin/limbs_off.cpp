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
#include "step_timer.h"
#include "game_graphics_gl.h"
#include "game_physics.h"
#include "init.h"
#include "event_handler.h"
#include "player.h"

const double MAX_FPS = 150;
const double STEPS_PER_SECOND = 600;

const phys_t GM = 628;
const phys_t R = 9.0;
const phys_t S = sqrt<phys_t> (GM / R) * 0.5;
const phys_t PR = 7.0;

int main(int argc, char *argv[]) {
    Screen* screen = Screen::getInstance();
    screen->setDrawingMode(Screen::DM_FRONT_TO_BACK | Screen::DM_SMOOTH, -1, false);
    TextureLoader* texLoader = TextureLoader::getInstance();
    GLuint tex = texLoader->loadTexture("test.png", true);
    Circle<phys_t> planetCircle = Circle<phys_t> (PR);
    AstroBody planet(GM, 2 * GM * PR * PR / 5, -0.05, &planetCircle);
    Circle<phys_t> characterCircle = Circle<phys_t> (1.0);
    Character character(50.0, state2p()(R, 0.0, 0.0, S), 2 * 50.0 * 1 * 1 / 5, 0.0,
            -5.0, &characterCircle);
    Player player(&character);

    GameUniverse universe(&planet, &character);
    Disk planetDisk(PR, 64);
    Disk planetSquare(PR, 4);
    Sprite characterSprite(tex, 1, 1);
    BodyGraphic planetGraphic(&planet, &planetDisk, 0.0, 0.0, 0.0),
        planetSquareGraphic(&planet, &planetSquare, 0.0, 0.0, 0.0),
        characterGraphic(&character, &characterSprite, 0.0, 0.0, 0.0);
    SDL_Event event;
    Init::readBindings(&player, "src/controllers.conf");
    bool quit = false;
    Uint32 time = SDL_GetTicks();
    StepTimer timer;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                break;
            }
            player.handle(event);
        }
        int steps = timer.getStepTime() * STEPS_PER_SECOND;
        timer.time(steps / STEPS_PER_SECOND);
        for (int i = 0; i < steps; ++i)
            universe.update(1.0 / STEPS_PER_SECOND);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        characterGraphic.draw();
        glColor3f(0.8, 0.4, 0.4);
        planetSquareGraphic.draw();
        glColor3f(0.4, 0.8, 0.4);
        planetGraphic.draw();
        SDL_GL_SwapBuffers();
        Uint32 d = SDL_GetTicks() - time;
        int w = 1000 / MAX_FPS - d;
        if (w > 0) {
            SDL_Delay(w);
            d = SDL_GetTicks() - time;
        }
        time += d;
        timer.targetTime(d / 1000.0);
    }
    return 0;
}
