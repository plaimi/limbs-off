/*
 * Copyright (C) 2011, 2012 Alexander Berntsen <alexander@plaimi.net>
 * Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
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

#ifndef GAME_H_
#define GAME_H_

#include "graphics/game_graphics_gl.h"
#include "physics/game_physics.h"
#include "player.h"
#include "step_timer.h"

class Game : public EventHandler {
public:
    /** Initialise the game. */
    Game(Screen* screen);
    ~Game();
    /** Handle input. */
    bool handle(const SDL_Event& event);
    /** Set existing_ to false. */
    void cease();
    /** Create objects. */
    void conceive(GLuint tex);
    /** Main game loop. */
    void main();
    void pause();
    void resume();
private:
    /** Max frames per second. */
    static const double MAX_FPS;
    /** Max player characters. */
    static const int MAX_PC;
    /** Max planets. */
    static const int MAX_PLAN;
    /** Max steps simulated per second. */
    static const double STEPS_PER_SECOND;
    /** Planet colour. */
    static const float COL_PLANET[];
    /** Planet gravity "mass". */
    static const phys_t GM;
    /** Orbit radius. */
    static const phys_t R;
    /** Orbit speed. */
    static const phys_t S;
    /** Planet radius. */
    static const phys_t PR;
    /** If the game is running or not. */
    bool existing_;
    /** If the game has been paused or not. */
    bool pause_;
    std::vector<AstroBody*> planets_;
    BackgroundModifier* backgroundModifier_;
    Camera* camera_;
    std::vector<Character*> characters_;
    std::vector<CharacterGraphic*> characterGraphics_;
    ColorModifier* planetColour_;
    GameUniverse* universe_;
    GraphicFixture* planetFixture_;
    Circle<phys_t>* planetCircle_;
    std::vector<Player*> players_;
    Screen* screen_;
    StackGraphic* scene_, * foreground_;
    StepTimer* timer_;
    Sprite* backgroundSprite_;
    TestDisk* planetDisk_;
    Uint32 time_;
};

#endif /* GAME_H_ */
