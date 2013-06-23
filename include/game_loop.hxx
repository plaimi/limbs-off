/*
 * Copyright (C) 2011, 2012 Alexander Berntsen <alexander@plaimi.net>
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

#ifndef GAME_LOOP_HXX_
#define GAME_LOPP_HXX_

#include <SDL/SDL.h>
#include "game.hxx"
#include "menu.hxx"
#include "event_code.hxx"

class GameLoop {
public:
    /** Number of steps simulated per second. */
    static const double _STEPS_PER_SECOND = 600;
    /** Max frames per second. */
    static const double _MAX_FPS = 200;
    /** Number of simulation steps between each event update. */
    static const int _EVENT_INTERVAL = 20;
    GameLoop();
    ~GameLoop();
    int run();
private:
    GameLoop(const GameLoop&);
    GameLoop& operator=(const GameLoop&);
    int numPlayers_, numCPUs_;
    bool running_, menuP_, inputP_;
    char* userInput_;
    EventCode activeInput_;
    Screen* screen_;
    Game* limbsOff_;
    InputFieldGraphic* inputFieldGraphic_;
    Menu menu_;
    int prevWidth_, prevHeight_;
    Uint8* keystate_;
    unsigned int stepCounter_;
    bool handleEvents();
};

#endif /* GAME_LOOP_HXX_ */
