/*
 * Copyright (C) 2011, 2012 Alexander Berntsen <alexander@plaimi.net>
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

#include <fstream>
#include <string>
#include "config_parser.h"
#include "player.h"

using namespace std;

void ConfigParser::readBindings(Player* player, const char* file) {
    string tmp;
    int code;
    ifstream in;
    in.open(file);
    if (in.fail()) {
        printf("ERROR: Could not open %s.\n", file);
        exit(1);
    }
    while (in) {
        getline(in, tmp);
        if (tmp == "[keyboard]") {
            for (int i = 1; i < NUM_ACTIONTYPE; ++i) {
                in >> tmp;
                in >> tmp;
                in >> code;
                player->bindKey((SDLKey) code, (ActionType) i);
            }
        }
        if (tmp == "[gamepad]") {
            for (int i = 1; i < NUM_ACTIONTYPE; ++i) {
                in >> tmp;
                in >> tmp;
                in >> code;
                player->bindJoyButton((Uint8) code, (ActionType) i);
            }
        }
    }
    in.close();
}
