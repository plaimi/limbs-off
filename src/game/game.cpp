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

#include <SDL/SDL.h>
#include "game.h"
#include "menu.h"
#include "config_parser.h"

const double Game::MAX_FPS = 200;
const double Game::STEPS_PER_SECOND = 600;
const int Game::MAX_PC = 16;
const int Game::MAX_PLAN = 1;
const float Game::COL_PLANET[] = { 0.4, 0.8, 0.4 };
const phys_t Game::GM = 628;
const phys_t Game::R = 9.0;
const phys_t Game::S = sqrt<phys_t> (GM / R) * 0.5;
const phys_t Game::PR = 7.0;

bool Game::handle(const SDL_Event& event) {
    // Input
    for (std::vector<Player*>::const_iterator i = players_.begin();
            i != players_.end(); ++i)
        (*i)->handle(event);
    return true;
}

Game::Game(Screen* screen) :
    screen_(screen) {
        TextureLoader* texLoader = TextureLoader::getInstance();
        GLuint tex = texLoader->loadTexture(PACKAGE_GFX_DIR "background.png",
                true);
        conceive(tex);
        for (std::vector<Character*>::const_iterator i = characters_.begin();
                i != characters_.end(); ++i)
            (*i)->addToUniverse(universe_);
        planetDisk_->addModifier(planetFixture_);
        planetDisk_->getDisk()->addModifier(planetColour_);
        // Bad hard coding incoming. The game is hard coded for three players.
        ConfigParser::readBindings(players_[0], 
                PACKAGE_CFG_DIR "controllers1.conf");
        ConfigParser::readBindings(players_[1], 
                PACKAGE_CFG_DIR "controllers2.conf");
        ConfigParser::readBindings(players_[2], 
                PACKAGE_CFG_DIR "controllers3.conf");
        // TODO: When the game is abstracted for more players, fix ^
        backgroundSprite_->addModifier(backgroundModifier_);
        scene_->addGraphic(backgroundSprite_);
        foreground_->addGraphic(planetDisk_);
        for (std::vector<CharacterGraphic*>::const_iterator i = 
                characterGraphics_.begin(); i != characterGraphics_.end(); ++i)
            foreground_->addGraphic(*i);
        foreground_->addModifier(camera_);
        scene_->addGraphic(foreground_);
        int j = SDL_NumJoysticks();
        for (int i = 0; i < j; ++i)
            SDL_JoystickOpen(i);
        time_ = SDL_GetTicks();
        pause_ = false;
}

Game::~Game() {
    for (std::vector<Character*>::const_iterator i = characters_.begin();
            i != characters_.end(); ++i)
        delete (*i);
    delete planetCircle_;
    for (std::vector<AstroBody*>::const_iterator i = planets_.begin();
            i != planets_.end(); ++i)
        delete (*i);
    delete universe_;
    delete backgroundSprite_;
    delete foreground_;
    delete planetColour_;
    delete planetDisk_;
    delete planetFixture_;
    delete scene_;
    delete camera_;
    delete backgroundModifier_;
    delete timer_;
}

void Game::cease() {
    existing_ = false;
}

void Game::conceive(GLuint tex) {
    // Characters
    // TODO: We have hard coded three players. We should not do that...
    phys_t angle = 2 * PI / 3;
    vector2p pos = { R, 0 }, vel = { 0, S }, a = vector2p::fromAngle(angle);
    for (int i = 0; i < 3; ++i) {
        characters_.push_back(new Character(state2p()(pos, vel), i * angle));
        players_.push_back(new Player(characters_[i]));
        characterGraphics_.push_back(new CharacterGraphic(characters_[i]));
        pos.rotate(a);
        vel.rotate(a);
    }
    // Planets
    planetCircle_ = new Circle<phys_t> (PR);
    planets_.push_back(new AstroBody(GM, 2 * GM * PR * PR / 5, -0.05, 
                planetCircle_));
    universe_ = new GameUniverse(planets_[0]);
    // Graphics
    backgroundSprite_ = new Sprite(tex, 1, 1);
    foreground_ = new StackGraphic();
    planetColour_ = new ColorModifier(COL_PLANET);
    planetDisk_ = new TestDisk(PR, 64);
    planetFixture_ = new GraphicFixture(planets_[0]);
    scene_ = new StackGraphic();
    // Camera
    camera_ = new Camera(characters_[0]->getState(), 0.5, 0.0);
    backgroundModifier_ = new BackgroundModifier(camera_);
    // Timer
    timer_ = new StepTimer();
}

void Game::main() {
    // Timer
    int steps = timer_->getStepTime() * STEPS_PER_SECOND;
    timer_->time(steps / STEPS_PER_SECOND);
    // Universe
    for (int i = 0; i < steps; ++i)
        universe_->update(1.0 / STEPS_PER_SECOND);
    // Camera
    vector2p planetPos = planets_[0]->getPosition(), up = vector2p()(0, 0);
    state2p camState = state2p()(0, 0, 0, 0);
    // Characters
    for (std::vector<Character*>::const_iterator i = characters_.begin(); 
            i != characters_.end(); ++i) {
        (*i)->update(steps / STEPS_PER_SECOND);
        // Camera
        camState += (*i)->getState();
        up += ((*i)->getState().p - planetPos).unit();
    }
    camState /= characters_.size();
    up /= characters_.size();
    phys_t camRadius = 0.0;
    for (std::vector<Character*>::const_iterator i = characters_.begin(); 
            i != characters_.end(); ++i) {
        phys_t r = (camState.p - (*i)->getState().p).squared();
        if (r > camRadius)
            camRadius = r;
    }
    camRadius = sqrt(camRadius);
    camera_->setTargetRadius(camRadius + 2);
    camera_->setTargetState(camState);
    vector2p camToPlanet = camState.p - planetPos;
    camera_->setTargetRotation(up.angle() * IN_DEG - 90.0, up.squared());
    camera_->update(steps / STEPS_PER_SECOND);
    // Draw
    scene_->draw();
    // Target time
    Uint32 delta = SDL_GetTicks() - time_;
    int wait = 1000 / MAX_FPS - delta;
    if (wait > 0) {
        SDL_Delay(wait);
        delta = SDL_GetTicks() - time_;
    }
    time_ += delta;
    timer_->targetTime(delta / 1000.0);
}

void Game::pause() {
    pause_ = true;
}

void Game::resume() {
    pause_ = false;
}
