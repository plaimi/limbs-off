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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL/SDL.h>
#include "game.hxx"
#include "get_font.hxx"
#include "get_texture.hxx"
#include "menu.hxx"
#include "config_parser.hxx"

const int Game::_MAX_PC = 16;
const int Game::_MAX_PLAN = 1;
const float Game::_COL_PLANET[] = { 0.4, 0.8, 0.4 };
const phys_t Game::_GM = 628;
const phys_t Game::_R = 9.0;
const phys_t Game::_S = sqrt<phys_t> (_GM / _R) * 0.5;
const phys_t Game::_PR = 7.0;

bool Game::handle(const SDL_Event& event) {
    // Input
    for (std::vector<Player*>::const_iterator i = players_.begin();
            i != players_.end(); ++i)
        (*i)->handle(event);
    return true;
}

Game::Game(Screen* screen, int numPlayers, int numCPUs) :
        screen_(screen),
        planets_(),
        backgroundModifier_(NULL),
        camera_(NULL),
        characters_(),
        characterGraphics_(),
        planetColour_(NULL),
        universe_(NULL),
        tex_(0),
        numPlayers_(numPlayers),
        numCPUs_(numCPUs),
        planetFixture_(NULL),
        massIndicatorLabels_(),
        planetCircle_(NULL),
        players_(),
        massIndicatorPosMods_(),
        scene_(NULL),
        foreground_(NULL),
        backgroundSprite_(NULL),
        planetDisk_(NULL),
        matCharBody_(NULL),
        matCharHead_(NULL),
        matCharLimbs_(NULL),
        matCharLimbsOff_(NULL),
        matPlanet_(NULL),
        massIndicators_(),
        massIndicatorGfx_() {
    tex_ = getTexture(PACKAGE_GFX_DIR "background.png");
    conceive();
    for (std::vector<Character*>::const_iterator i = characters_.begin();
            i != characters_.end(); ++i)
        (*i)->addToUniverse(universe_);
    planetDisk_->addModifier(planetFixture_);
    planetDisk_->getDisk()->addModifier(planetColour_);
    // Bad hard coding incoming. The game is hard coded for three players.
    switch (numPlayers_) {
    default:
        if (numPlayers_ < 1)
            break;
        ;
    case 3:
        ConfigParser::readBindings(players_[2],
                PACKAGE_CFG_DIR "controllers3.conf");
    case 2:
        ConfigParser::readBindings(players_[1],
                PACKAGE_CFG_DIR "controllers2.conf");
    case 1:
        ConfigParser::readBindings(players_[0],
                PACKAGE_CFG_DIR "controllers1.conf");
    }
    // TODO: When the game is abstracted for more players, fix ^
#if VERBOSE
    printf("controllers player 1:\n"
            "\t left:       \t\t        left arrow\n"
            "\t right:      \t\t        right arrow\n"
            "\t jump:       \t\t        up arrow\n"
            "\t crouch:     \t\t        down arrow\n"
            "\t left punch: \t\t        w\n"
            "\t right punch:\t\t        v\n"
            "\t suicide:    \t\t        end\n"
            "\n"
            "controllers player 2:\n"
            "\t left:        \t\t        a\n"
            "\t right:       \t\t        e\n"
            "\t jump:        \t\t        ,\n"
            "\t crouch:      \t\t        o\n"
            "\t left punch:  \t\t        .\n"
            "\t right punch: \t\t        p\n"
            "\t suicide:     \t\t        delete\n"
            "\n"
            "controllers player 3:\n"
            "\t left:        \t\t        left button\n"
            "\t right:       \t\t        right button\n"
            "\t jump:        \t\t        up button\n"
            "\t crouch:      \t\t        down button\n"
            "\t left punch:  \t\t        cross\n"
            "\t right punch: \t\t        circle\n"
            "\t suicide:     \t\t        triangle\n\n"
            "\t note: these buttons are for the Sony Sixaxis\n"
            "\t and are not guaranteed to work with other gamepads.\n\n");
    printf("press escape to bring up the menu again.\n"
            "press escape again to close it.\n\n\n\n");
#endif
    backgroundSprite_->addModifier(backgroundModifier_);
    scene_->addGraphic(backgroundSprite_);
    foreground_->addGraphic(planetDisk_);
    for (std::vector<CharacterGraphic*>::const_iterator i =
            characterGraphics_.begin(); i != characterGraphics_.end(); ++i)
        foreground_->addGraphic(*i);
    foreground_->addModifier(camera_);
    scene_->addGraphic(foreground_);
    for (std::vector<MassIndicatorGraphic*>::const_iterator i =
            massIndicatorGfx_.begin();
            i != massIndicatorGfx_.end(); ++i)
        scene_->addGraphic(*i);
}

Game::~Game() {
    for (std::vector<Character*>::const_iterator i = characters_.begin();
            i != characters_.end(); ++i)
        delete (*i);
    for (std::vector<Player*>::const_iterator i = players_.begin();
            i != players_.end(); ++i)
        delete (*i);
    for (std::vector<CharacterGraphic*>::const_iterator i =
            characterGraphics_.begin(); i != characterGraphics_.end(); ++i)
        delete (*i);
    for (std::vector<Label*>::const_iterator i = massIndicatorLabels_.begin();
            i != massIndicatorLabels_.end(); ++i)
        delete (*i);
    for (std::vector<MassIndicator*>::const_iterator i =
            massIndicators_.begin(); i != massIndicators_.end(); ++i)
        delete (*i);
    for (std::vector<MassIndicatorGraphic*>::const_iterator i =
            massIndicatorGfx_.begin(); i != massIndicatorGfx_.end(); ++i)
        delete (*i);
    for (std::vector<PositionModifier*>::iterator i =
            massIndicatorPosMods_.begin(); i != massIndicatorPosMods_.end();
            ++i)
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
}

void Game::conceive() {
    // Characters
    phys_t angle = 2 * PI / numPlayers_;
    vector2p pos = { _R, 0 }, vel = { 0, _S }, a = vector2p::fromAngle(angle);
    matCharBody_ = new Material(100.0, 0.5);
    matCharHead_ = new Material(10000.0, 0.1);
    matCharLimbs_ = new Material(50000.0, 1.5);
    matCharLimbsOff_ = new Material(500.0, 1.5);
    char font[256];
    getFont(font, sizeof(font));
    for (int i = 0; i < numPlayers_; ++i) {
        characters_.push_back(new Character(state2p()(pos, vel), i * angle,
                matCharBody_, matCharHead_, matCharLimbs_, matCharLimbsOff_));
        players_.push_back(new Player(characters_[i]));
        characterGraphics_.push_back(new CharacterGraphic(characters_[i]));
        char mass [4];
        snprintf(mass, sizeof(mass), "%.0f", characters_[i]->getMass());
        massIndicatorLabels_.push_back(new Label(font, mass, 74, .05,
                    .02));
        massIndicators_.push_back(new MassIndicator(i));
        massIndicatorGfx_.push_back(new MassIndicatorGraphic(0.05f, 0.02f,
                    massIndicators_[i], massIndicatorLabels_[i]));
        massIndicatorPosMods_.push_back(new PositionModifier(i + 1,
                    numPlayers_, true, -0.9));
        massIndicatorGfx_[i]->addModifier(massIndicatorPosMods_[i]);
        massIndicatorGfx_[i]->addModifier(
                characterGraphics_[i]->getColourModifier());
        pos.rotate(a);
        vel.rotate(a);
    }
    // Planets
    planetCircle_ = new Circle<phys_t> (_PR);
    matPlanet_ = new Material(100.0, 50);
    planets_.push_back(new AstroBody(_GM, 2 * _GM * _PR * _PR / 5, -0.05, 
                planetCircle_, matPlanet_));
    universe_ = new GameUniverse(planets_[0]);
    // Graphics
    backgroundSprite_ = new Sprite(tex_, 1, 1);
    foreground_ = new StackGraphic();
    planetColour_ = new ColorModifier(_COL_PLANET);
    planetDisk_ = new TestDisk(_PR, 64);
    planetFixture_ = new GraphicFixture(planets_[0]);
    scene_ = new StackGraphic();
    // Camera
    camera_ = new Camera(characters_[0]->getState(), 0.5, 0.0);
    backgroundModifier_ = new BackgroundModifier(camera_);
}

void Game::update(phys_t dt) {
    universe_->update(dt);
    for (std::vector<Character*>::const_iterator it = characters_.begin();
            it != characters_.end(); ++it)
        (*it)->update(dt);
    for (std::vector<CharacterGraphic*>::const_iterator it =
            characterGraphics_.begin(); it != characterGraphics_.end(); ++it)
        (*it)->update();
}

void Game::updateCamera(GLfloat dt) {
    // Camera
    vector2p planetPos = planets_[0]->getPosition(), up = vector2p()(0, 0);
    state2p camState = state2p()(0, 0, 0, 0);
    // Characters
    int i = 0;
    double j = 0.0;
    for (std::vector<Character*>::const_iterator it = characters_.begin();
            it != characters_.end(); ++it) {
        // Camera
        if ((*it)->isDead())
            continue;
        camState += (*it)->getState();
        vector2p k = (*it)->getState().p - planetPos;
        phys_t l = 1 / k.length();
        up += k * l * l;
        j += l;
        ++i;
    }
    phys_t camRadius = 0.0;
    // End if players are dead.
    if (i == 0) {
        ++i;
        camRadius = 100.0;
    }
    if (j == 0)
        ++j;
    camState /= i;
    up /= j;
    for (std::vector<Character*>::const_iterator it = characters_.begin();
            it != characters_.end(); ++it) {
        if ((*it)->isDead())
            continue;
        phys_t r = (camState.p - (*it)->getState().p).squared();
        if (r > camRadius)
            camRadius = r;
    }
    camRadius = sqrt(camRadius);
    camera_->setTargetRadius(camRadius + 2);
    camera_->setTargetState(camState);
    vector2p camToPlanet = camState.p - planetPos;
    camera_->setTargetRotation(up.angle() * IN_DEG - 90.0, up.squared());
    camera_->update(dt);
}

void Game::draw() {
    scene_->draw();
    int i = 0;
    char mass [4];
    for (std::vector<Character*>::const_iterator it = characters_.begin();
            it != characters_.end(); ++it, ++i) {
        snprintf(mass, sizeof(mass), "%.0f", (*it)->getMass());
        massIndicatorLabels_[i]->setText(mass);
    }
}
