/*
 * Copyright (C) 2012 Stian Ellingsen <stiell@stiell.org>
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

#include <GL/gl.h>
#include <SDL/SDL.h>
#include "game.h"
#include "menu.h"

int main(int argc, char *argv[]) {
    Screen::setVideoMode(1024, 768, 32);
    Screen* screen = Screen::getInstance();
    screen->setDrawingMode(Screen::DM_FRONT_TO_BACK | Screen::DM_SMOOTH, -1);
    SDL_Event event;
    Game limbsOff(screen);
    Menu menu;
    MenuGraphic menugraphic(&menu);
    bool running = true;
    bool menuP = false;
    int prevWidth_, prevHeight_;
    Uint8 *keystate = SDL_GetKeyState(NULL);
    while (running) {
        // Events
        while (SDL_PollEvent(&event)) {
            //Quit
            if (event.type == SDL_QUIT) {
                    running = false;
                    limbsOff.cease();
            }
            // Screen
            if (keystate[SDLK_LALT] && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                // Enter/leave fullscreen
                if (!screen->getFullscreen()) {
                    prevWidth_ = screen->getSurfaceWidth();
                    prevHeight_ = screen->getSurfaceHeight();
                    screen->setVideoMode(0, 0, 32, true);
                } else
                    screen->setVideoMode(prevWidth_, prevHeight_, 32, false);
            }
            if (screen->handle(event))
                continue;
            // Menu
            if (event.type == SDL_KEYDOWN &&
                    event.key.keysym.sym == SDLK_ESCAPE)
                menuP = !menuP;
            if (menuP)
                menuP = menu.handle(event);
            // Game
            else
                limbsOff.handle(event);
        }
        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        // Menu
        if (menuP)
            menugraphic.draw();
        // Game
        limbsOff.main();
        // Swap buffers
        SDL_GL_SwapBuffers();
    }
    return 0;
}
