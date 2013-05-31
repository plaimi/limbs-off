/*
 * Copyright (C) 2011, 2012, 2013 Alexander Berntsen <alexander@plaimi.net>
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
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
#include "repeat.hxx"
#include "get_font.hxx"
#include "game_loop.hxx"
#include "event_code.hxx"
#include "step_timer.hxx"

GameLoop::GameLoop() :
        screen_(Screen::getInstance()),
        prevWidth_(0),
        prevHeight_(0),
        limbsOff_(NULL),
        running_(true),
        menuP_(true),
        inputP_(false),
        userInput_(),
        numPlayers_(1),
        numCPUs_(0),
        activeInput_(NUM_EVENT_CODE),
        keystate_(SDL_GetKeyState(NULL)),
        stepCounter_(0),
        menu_(),
        inputFieldGraphic_(NULL) {
    // This is necessary for the input field
    SDL_EnableUNICODE(1);
    // Stores user input from the input field in menu
    userInput_ = (char*) malloc(32);
    userInput_[0] = '\0';
}

GameLoop::~GameLoop() {
    free(userInput_);
}

bool GameLoop::handleEvents() {
    SDL_Event event;
    stepCounter_ = 0;
    // Events
    while (SDL_PollEvent(&event)) {
        //Quit
        if (event.type == SDL_QUIT)
                running_ = false;
        // Screen
        if (keystate_[SDLK_LALT] && event.type == SDL_KEYDOWN && 
                event.key.keysym.sym == SDLK_RETURN) {
            // Enter/leave fullscreen
            if (!screen_->getFullscreen()) {
                prevWidth_ = screen_->getSurfaceWidth();
                prevHeight_ = screen_->getSurfaceHeight();
                screen_->setVideoMode(0, 0, 32, true);
            } else
                screen_->setVideoMode(prevWidth_, prevHeight_, 32, false);
        }
        if (screen_->handle(event))
            continue;
        // Menu
        if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE && !inputP_)
            menuP_ = !menuP_;
        // Input field
        if (inputP_) {
            if (!menu_.getChoice(event, userInput_)) {
                // Update the logic's text
                menu_.getInputField()->setText(userInput_);
                // Update the graphic's text
                inputFieldGraphic_->setText();
            }
            else {
                inputP_ = false;
                menuP_ = true;
                switch (activeInput_) {
                case CHANGE_PLAYERS:
                    numPlayers_ = max(strtol(userInput_, 0, 10), 1L);
                    break;
                case CHANGE_CPUS:
                    numCPUs_ = max(strtol(userInput_, 0, 10), 1L);
                    break;
                default:
                    ;
                }
                userInput_[0] = '\0';
            }
        }
        else if (menuP_)
            menuP_ = !menu_.handle(event);
        // Game
        else
            if (limbsOff_)
                limbsOff_->handle(event);
        // Userevents
        if (event.type == SDL_USEREVENT) {
            switch (event.user.code) {
            case NEW_GAME:
                if (limbsOff_)
                    delete limbsOff_;
                limbsOff_ = new Game(screen_, numPlayers_, numCPUs_);
                break;
            case CHANGE_PLAYERS:
                activeInput_ = CHANGE_PLAYERS;
                inputP_ = true;
                break;
            case CHANGE_CPUS:
                activeInput_ = CHANGE_CPUS;
                inputP_ = true;
                inputP_ = true;
                break;
            default:
                ;
            }
        }
    }
}

int GameLoop::run() {
    if (!screen_)
        return 1;
    screen_->setDrawingMode(Screen::_DM_FRONT_TO_BACK | Screen::_DM_SMOOTH, 
            -1);
    StepTimer timer;
    int j = SDL_NumJoysticks();
    for (int i = 0; i < j; ++i)
        SDL_JoystickOpen(i);
    MenuGraphic menugraphic(&menu_);
    char font[256];
    getFont(font, sizeof(font));
    inputFieldGraphic_ = new InputFieldGraphic(font, menu_.getInputField());
    Uint32 time = SDL_GetTicks();
    while (running_) {
        int steps = timer.getStepTime() * _STEPS_PER_SECOND;
        timer.time(steps / _STEPS_PER_SECOND);
        if (limbsOff_) {
            REPEAT(steps, I) {
                limbsOff_->update(1.0 / _STEPS_PER_SECOND);
                if (++stepCounter_ == _EVENT_INTERVAL)
                    handleEvents();
            }
            limbsOff_->updateCamera(steps / _STEPS_PER_SECOND);
        } else
            handleEvents();
        // Draw
        glClear(GL_COLOR_BUFFER_BIT);
        // Input field
        if (inputP_)
            inputFieldGraphic_->draw();
        // Menu
        if (menuP_)
            menugraphic.draw();
        // Game
        if (limbsOff_)
            limbsOff_->draw();
        Uint32 delta = SDL_GetTicks() - time;
        int wait = 1000 / _MAX_FPS - delta;
        if (wait > 0) {
            SDL_Delay(wait);
            delta = SDL_GetTicks() - time;
        }
        time += delta;
        timer.targetTime(delta / 1000.0);
        // Swap buffers
        SDL_GL_SwapBuffers();
    }
    return 0;
}
