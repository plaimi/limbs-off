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

#include <fstream>
#include <string>
#include "init.h"
#include "player.h"

using namespace std;

void Init::readBindings(Player* player, const char* file) {
    string tmp;
    int keyCode, actionType;
    ifstream in;
    in.open(file);
    if (in.fail()) {
        printf("ERROR: Could not open %s.\n", file);
        exit(1);
    }
    while (in) {
        getline(in, tmp, ':');
        in >> keyCode;
        in >> actionType;
        player->bindKey((SDLKey) keyCode, (ActionType) actionType);
    }
    in.close();
}
